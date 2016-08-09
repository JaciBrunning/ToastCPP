#include "toast/http/server.hpp"

#include "toast/memory.hpp"

using namespace Toast::Concurrent;
using namespace Toast::HTTP;

static DirHandler globalResourceHandler("res", "resources");

static Server *get_server(struct mg_connection *nc) {
	return (Server *)nc->mgr->user_data;
}

static void set_server(struct mg_connection *nc, Server *serv) {
	nc->mgr->user_data = (void *)serv;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
	Server *server = get_server(nc);

	if (server != NULL) {
		// WebSocket
		if (ev == MG_EV_WEBSOCKET_HANDSHAKE_REQUEST) {
			struct http_message *msg = (struct http_message *) ev_data;
			nc->user_data = msg;
		} else if (ev == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
			// Websocket Opened
			struct http_message *msg = (struct http_message *) nc->user_data;
			server->_webSocketReady(nc, msg);
		} else if (ev == MG_EV_WEBSOCKET_FRAME) {
			// Websocket Data Received
			struct websocket_message *wm = (struct websocket_message *) ev_data;
			server->_webSocketData(nc, string((char *)wm->data, wm->size));
		} else if (ev == MG_EV_CLOSE) {
			// Websocket Closed
			if (nc->flags & MG_F_IS_WEBSOCKET) 
				server->_webSocketClosed(nc);
		}

		// HTTP
		if (ev == MG_EV_HTTP_REQUEST) {
			if (!server->_handleRequest(nc, (struct http_message *) ev_data)) {
				// 404
				std::string reason = "404: Page not found";
				mg_send_head(nc, 404, reason.size(), "Content-Type: text/plain\r\nConnection: close");
				mg_send(nc, reason.c_str(), reason.size());
				nc->flags |= MG_F_SEND_AND_CLOSE;
			}
		}
	}
}

string Toast::HTTP::htmlEntities(string data) {
	string buffer;
	buffer.reserve(data.size());

	for (size_t pos = 0; pos != data.size(); ++pos) {
		switch (data[pos]) {
		case '&':  buffer.append("&amp;");       break;
		case '\"': buffer.append("&quot;");      break;
		case '\'': buffer.append("&apos;");      break;
		case '<':  buffer.append("&lt;");        break;
		case '>':  buffer.append("&gt;");        break;
		default:   buffer.append(1, data[pos]); break;
		}
	}

	return buffer;
}

class SHMSocket : public WebSocketHandler {
public:
	void on_ready(WebSocket *ws) { }
	void on_message(WebSocket *ws, string data) {
		ws->send(Encoding::base64_encode(Toast::Memory::shared()->get_store(), Toast::Memory::SharedPool::SIZE));
	}
	void on_closed(WebSocket *ws) { }
};
SHMSocket _socket;

class SHMSocketHandler : public Handler {
public:
	void setup() {
		register_web_socket("/socket/shared_memory", &_socket);
	}
};

// Server
Server::Server(int port_) : stopped(false), mgr(), websockets(), port(port_) {
	register_handler(&globalResourceHandler);
}
Server::~Server() {
	stop();
}

void Server::start(int pollTiming) {
	mg_mgr_init(&mgr, NULL);

	mgr.user_data = (Server *)this;
	struct mg_connection *conn = mg_bind(&mgr, to_string(port).c_str(), ev_handler);
	mg_set_protocol_http_websocket(conn);

	stopped = false;
	for (;;) {
		mg_mgr_poll(&mgr, pollTiming);
	}

	stop();
}

void Server::stop() {
	mg_mgr_free(&mgr);
	stopped = true;
}

void Server::register_handler(Handler *handler) {
	handler->set_server(this);
	handler->setup();
	handlers.push_back(handler);
}

void Server::enable_memory_socket() {
	SHMSocketHandler *_hand = new SHMSocketHandler();
	register_handler(_hand);
}

int Server::_handleRequest(struct mg_connection *conn, struct http_message *msg) {
	Request req(conn, msg);
	
	mutex.lock();
	currentRequests[conn] = &req;
	mutex.unlock();

	Response *response = handleRequest(&req);

	mutex.lock();
	currentRequests.erase(conn);
	mutex.unlock();

	if (response == NULL)
		return 0;
	else {
		req.write_response(response);
		delete response;
		return 1;
	}
}

bool Server::handles(string method, string url) {
	vector<Handler *>::iterator it;
	for (it = handlers.begin(); it != handlers.end(); it++) {
		if ((*it)->handles(method, url)) {
			return true;
		}
	}

	return false;
}

Response *Server::handleRequest(Request *request) {
	Response *response;
	vector<Handler *>::iterator it;
	for (it = handlers.begin(); it != handlers.end(); it++) {
		Handler *handler = *it;
		response = handler->process(request);

		if (response != NULL)
			return response;
	}
	return NULL;
}

void Server::_webSocketReady(struct mg_connection *conn, struct http_message *msg) {
	WebSocket *ws = new WebSocket(conn, msg);
	websockets.add(ws);
	websockets.clean();

	vector<Handler *>::iterator it;
	for (it = handlers.begin(); it != handlers.end(); it++) {
		(*it)->webSocketReady(ws);
	}
}

int Server::_webSocketData(struct mg_connection *conn, string data) {
	WebSocket *websocket = websockets.get(conn);

	if (websocket != NULL) {
		websocket->append_data(data);

		string fullPacket = websocket->flush_data();
		vector<Handler *>::iterator it;
		for (it = handlers.begin(); it != handlers.end(); it++) {
			(*it)->webSocketData(websocket, fullPacket);
		}

		if (websocket->is_closed()) {
			websockets.remove(websocket);
			return 0;
		}
		else {
			return -1;
		}
	}
	else {
		return 0;
	}
}

void Server::_webSocketClosed(struct mg_connection *conn) {
	WebSocket *websocket = websockets.get(conn);
	websocket->close();

	vector<Handler *>::iterator it;
	for (it = handlers.begin(); it != handlers.end(); it++) {
		(*it)->webSocketClosed(websocket);
	}

	websockets.clean();
}

WebSocketContainer *Server::get_web_sockets() {
	return &websockets;
}
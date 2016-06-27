#include "toast/http/server.hpp"

using namespace Toast::Concurrent;
using namespace Toast::HTTP;

static Server *get_server(struct mg_connection *nc) {
	return (Server *)nc->mgr->user_data;
}

static void set_server(struct mg_connection *nc, Server *serv) {
	nc->mgr->user_data = (void *)serv;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
	Server *server = get_server(nc);

	//if (server != NULL) {
		if (ev == MG_EV_HTTP_REQUEST) {
			server->_handleRequest(nc, (struct http_message *) ev_data);
		}
	//}
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

// Server
Server::Server(int port_) : stopped(false), mgr(), websockets(), port(port_) { }
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

void Server::registerHandler(Handler *handler) {
	handler->setServer(this);
	handler->setup();
	handlers.push_back(handler);
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
		req.writeResponse(response);
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

WebSocketContainer *Server::getWebSockets() {
	return &websockets;
}
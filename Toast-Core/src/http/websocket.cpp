#include "toast/http/websocket.hpp"

using namespace std;
using namespace Toast::HTTP;
using namespace Toast::Concurrent;

// Container

WebSocketContainer::WebSocketContainer() : id(0) { }
WebSocketContainer::~WebSocketContainer() {
	vector<WebSocket *> toDelete;

	map<struct mg_connection *, WebSocket *>::iterator it;

	for (it = websockets.begin(); it != websockets.end(); it++) {
		toDelete.push_back((*it).second);
	}

	vector<WebSocket *>::iterator vit;

	for (vit = toDelete.begin(); vit != toDelete.end(); vit++) {
		remove(*vit);
	}
}

void WebSocketContainer::add(WebSocket *ws) {
	if (ws == NULL) return;

	mutex.lock();
	int newId = id++;
	mutex.unlock();
	ws->set_id(newId);

	struct mg_connection *connection = ws->get_connection();

	mutex.lock();
	if (websockets.find(connection) != websockets.end()) {
		remove(websockets[connection], false);
	}

	websocketsById[ws->get_id()] = ws;
	websockets[connection] = ws;
	mutex.unlock();
}

WebSocket *WebSocketContainer::get_by_id(int id_) {
	if (websocketsById.find(id_) != websocketsById.end()) 
		return websocketsById[id_];
	return NULL;
}

void WebSocketContainer::send(string data) {
	vector<WebSocket *> toClean;
	map<struct mg_connection *, WebSocket *>::iterator it;

	mutex.lock();
	for (it = websockets.begin(); it != websockets.end(); it++) {
		WebSocket *websocket = (*it).second;

		websocket->send(data);
	}
	mutex.unlock();

	clean();
}

void WebSocketContainer::remove(WebSocket *ws, bool lock) {
	struct mg_connection *connection = ws->get_connection();

	if (lock) mutex.lock();

	if (websockets.find(connection) != websockets.end()) {
		ws->remove_container(this);
		websockets.erase(connection);
		websocketsById.erase(ws->get_id());

		ws->close();
		ws->notify_containers();
		delete ws;
	}

	if (lock) mutex.unlock();
}

WebSocket *WebSocketContainer::get(struct mg_connection *connection) {
	if (websockets.find(connection) != websockets.end())
		return websockets[connection];
	
	return NULL;
}

void WebSocketContainer::clean() {
	vector<WebSocket *> toDelete;
	map<struct mg_connection *, WebSocket *>::iterator it;

	mutex.lock();
	for (it = websockets.begin(); it != websockets.end(); it++) {
		if ((*it).second->is_closed()) {
			toDelete.push_back((*it).second);
		}
	}

	vector<WebSocket *>::iterator vit;
	for (vit = toDelete.begin(); vit != toDelete.end(); vit++) {
		remove(*vit, false);
	}
	mutex.unlock();
}

// WebSocket Instance
WebSocket::WebSocket(struct mg_connection *connection_, struct http_message *msg_) 
	: connection(connection_), message(msg_), req(connection, message), closed(false), data(""), id(-1) { }

Request *WebSocket::request() {
	return &req;
}

struct mg_connection *WebSocket::get_connection() {
	return connection;
}

void WebSocket::set_id(int id_) { id = id_; }
int WebSocket::get_id() { return id; }

void WebSocket::send(string data, int opcode) {
	if (is_closed()) return;

	mutex.lock();
	mg_send_websocket_frame(connection, opcode, data.c_str(), data.size());
	mutex.unlock();
}

void WebSocket::append_data(string data_) {
	data += data_;
}

string WebSocket::flush_data() {
	string old = "";
	data.swap(old);
	return old;
}

void WebSocket::close() {
	closed = true;
}

bool WebSocket::is_closed() {
	return closed;
}

void WebSocket::add_container(WebSocketContainer *websockets) {
	mutex.lock();
	containers.push_back(websockets);
	mutex.unlock();
}

void WebSocket::remove_container(WebSocketContainer *websockets) {
	mutex.lock();
	vector<WebSocketContainer *>::iterator it;

	for (it = containers.begin(); it != containers.end(); it++) {
		if (*it == websockets) {
			containers.erase(it);
			break;
		}
	}
	mutex.unlock();
}

void WebSocket::notify_containers() {
	vector<WebSocketContainer *>::iterator it;

	mutex.lock();
	for (it = containers.begin(); it != containers.end(); it++) {
		(*it)->remove(this);
	}
	mutex.unlock();
}
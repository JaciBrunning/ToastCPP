#include "toast/http/handler.hpp"

#include "toast/util.hpp"
#include "toast/filesystem.hpp"

#include <fstream>
#include <sstream>

using namespace std;
using namespace Toast::HTTP;

Handler::Handler() : server(NULL), prefix("") { }

Handler::~Handler() {
	map<string, RequestHandlerBase *>::iterator it;

	for (it = routes.begin(); it != routes.end(); it++)
		delete (*it).second;

	routes.clear();
}

void Handler::setup() { }

void Handler::set_server(Server *serv) {
	server = serv;
}

void Handler::webSocketReady(WebSocket *ws) {
	map<string, WebSocketHandler *>::iterator it;
	for (it = wsRoutes.begin(); it != wsRoutes.end(); it++) {
		if (ws->request()->match(it->first)) {
			it->second->on_ready(ws);
		}
	}
}

void Handler::webSocketData(WebSocket *ws, string data) {
	map<string, WebSocketHandler *>::iterator it;
	for (it = wsRoutes.begin(); it != wsRoutes.end(); it++) {
		if (ws->request()->match(it->first)) {
			it->second->on_message(ws, data);
		}
	}
}

void Handler::webSocketClosed(WebSocket *ws) {
	map<string, WebSocketHandler *>::iterator it;
	for (it = wsRoutes.begin(); it != wsRoutes.end(); it++) {
		if (ws->request()->match(it->first)) {
			it->second->on_closed(ws);
		}
	}
}

void Handler::register_web_socket(string route, WebSocketHandler *handler) {
	string key = "GET:" + prefix + route;
	wsRoutes[key] = handler;
}

void Handler::web_socket(string route, WebSocketHandler *handler) {
	register_web_socket(route, handler);
}

bool Handler::handles(string method, string url) {
	string key = method + ":" + url;
	return (routes.find(key) != routes.end());
}

void Handler::pre_process(Request *req, Response *resp) { }

Response *Handler::process(Request *request) {
	Response *response = NULL;

	map<string, RequestHandlerBase *>::iterator it;
	for (it = routes.begin(); it != routes.end(); it++) {
		if (request->match(it->first)) {
			response = it->second->process(request);
			break;
		}
	}

	return response;
}

void Handler::post_process(Request *req, Response *resp) { }

Response *Handler::handle_request(Request *req) {
	Response *response = process(req);
	if (response != NULL) post_process(req, response);
	return response;
}

void Handler::set_prefix(string pre) {
	prefix = pre;
}

void Handler::register_route(string httpMethod, string route, RequestHandlerBase *handler) {
	string key = httpMethod + ":" + prefix + route;
	routes[key] = handler;
	urls.push_back(prefix + route);
}

Response *Handler::serverInternalError(string message) {
	StreamResponse *response = new StreamResponse;

	response->set_code(HTTP_INTERNAL_ERROR);
	*response << "[500] Server internal error: " << message;

	return response;
}

vector<string> Handler::get_urls() {
	return urls;
}

// Basic HTTP Handler
HTTPHandler::HTTPHandler() { }
void HTTPHandler::pre_process(Request *req, Response *resp) {
	resp->set_header("Content-Type", "text/html");
}

// Directory Handler
DirHandler::DirHandler(string uri_base, string dir) : _dir(dir), _uri_base(uri_base) {}
Response *DirHandler::process(Request *req) {
	std::string uri = req->get_url().substr(1);
	if (!starts_with(uri, _uri_base + "/")) return NULL;

	std::string target = _dir + "/" + uri.substr(4);

	ifstream is;
	is.open(target.c_str(), ios::binary);
	if (!is.good()) return NULL;

	StreamResponse *res = new StreamResponse;
	(*res) << is.rdbuf();
	is.close();

	res->set_header("Content-Type", mime_type(Filesystem::extension(target), "text/plain"));

	return res;
}
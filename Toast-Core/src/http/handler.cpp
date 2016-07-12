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

void Handler::setServer(Server *serv) {
	server = serv;
}

void Handler::webSocketReady(WebSocket *ws) {
	map<string, WebSocketHandler *>::iterator it;
	for (it = wsRoutes.begin(); it != wsRoutes.end(); it++) {
		if (ws->request()->match(it->first)) {
			it->second->onReady(ws);
		}
	}
}

void Handler::webSocketData(WebSocket *ws, string data) {
	map<string, WebSocketHandler *>::iterator it;
	for (it = wsRoutes.begin(); it != wsRoutes.end(); it++) {
		if (ws->request()->match(it->first)) {
			it->second->onMessage(ws, data);
		}
	}
}

void Handler::webSocketClosed(WebSocket *ws) {
	map<string, WebSocketHandler *>::iterator it;
	for (it = wsRoutes.begin(); it != wsRoutes.end(); it++) {
		if (ws->request()->match(it->first)) {
			it->second->onClosed(ws);
		}
	}
}

void Handler::registerWebSocket(string route, WebSocketHandler *handler) {
	string key = "GET:" + prefix + route;
	wsRoutes[key] = handler;
}

void Handler::webSocket(string route, WebSocketHandler *handler) {
	registerWebSocket(route, handler);
}

bool Handler::handles(string method, string url) {
	string key = method + ":" + url;
	return (routes.find(key) != routes.end());
}

void Handler::preProcess(Request *req, Response *resp) { }

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

void Handler::postProcess(Request *req, Response *resp) { }

Response *Handler::handleRequest(Request *req) {
	Response *response = process(req);
	if (response != NULL) postProcess(req, response);
	return response;
}

void Handler::setPrefix(string pre) {
	prefix = pre;
}

void Handler::registerRoute(string httpMethod, string route, RequestHandlerBase *handler) {
	string key = httpMethod + ":" + prefix + route;
	routes[key] = handler;
	urls.push_back(prefix + route);
}

Response *Handler::serverInternalError(string message) {
	StreamResponse *response = new StreamResponse;

	response->setCode(HTTP_INTERNAL_ERROR);
	*response << "[500] Server internal error: " << message;

	return response;
}

vector<string> Handler::getUrls() {
	return urls;
}

// Basic HTTP Handler
HTTPHandler::HTTPHandler() { }
void HTTPHandler::preProcess(Request *req, Response *resp) {
	resp->setHeader("Content-Type", "text/html");
}

// Directory Handler
DirHandler::DirHandler(string uri_base, string dir) : _dir(dir), _uri_base(uri_base) {}
Response *DirHandler::process(Request *req) {
	std::string uri = req->getUrl().substr(1);
	if (!starts_with(uri, _uri_base + "/")) return NULL;

	std::string target = _dir + "/" + uri.substr(4);

	ifstream is;
	is.open(target.c_str(), ios::binary);
	if (!is.good()) return NULL;

	StreamResponse *res = new StreamResponse;
	(*res) << is.rdbuf();
	is.close();

	res->setHeader("Content-Type", mime_type(Filesystem::extension(target), "text/plain"));

	return res;
}
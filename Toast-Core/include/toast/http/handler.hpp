#pragma once

#include "toast/http/request.hpp"
#include "toast/http/response.hpp"
#include "toast/http/websocket.hpp"
#include "toast/library.hpp"

#include <map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define route(httpMethod, url, handlerType, method) \
    registerRoute(httpMethod, url, new RequestHandler<handlerType, StreamResponse>(this, &handlerType::method ));

#define route_type(httpMethod, url, handlerType, method, responseType) \
    registerRoute(httpMethod, url, new RequestHandler<handlerType, responseType>(this, &handlerType::method ));

namespace Toast {
	namespace HTTP {
		class Server;	// Fwd Declaration for Toast::HTTP::Server

		class Handler {
		public:
			API Handler();
			API virtual ~Handler();

			API virtual void setServer(Server *server);
			API virtual void preProcess(Request *request, Response *response);
			API virtual Response *process(Request *request);
			API virtual void postProcess(Request *request, Response *response);

			API virtual Response *handleRequest(Request *request);
			API void setPrefix(string prefix);

			API virtual void webSocketReady(WebSocket *websocket);
			API virtual void webSocketData(WebSocket *websocket, string data);
			API virtual void webSocketClosed(WebSocket *websocket);
			API virtual void registerWebSocket(string route, WebSocketHandler *handler);
			API virtual void webSocket(string route, WebSocketHandler *handler);	// same as registerWebSocket

			API virtual void registerRoute(string httpMethod, string route, RequestHandlerBase *handler);
			API virtual void setup();

			API virtual Response *serverInternalError(string message);
			API virtual bool handles(string method, string url);
			API vector<string> getUrls();

		protected:
			Server *server;
			string prefix;
			map<string, RequestHandlerBase* > routes;
			map<string, WebSocketHandler *> wsRoutes;
			vector<string> urls;
		};

		class HTTPHandler : public Handler {
		public:
			API HTTPHandler();
			API void preProcess(Request *request, Response *response);
		};

		class DirHandler : public Handler {
		public:
			API DirHandler(string uri_base, string dir);
			API virtual Response *process(Request *request);
		private:
			string _uri_base;
			string _dir;
		};
	}
}
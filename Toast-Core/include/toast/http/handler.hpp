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

#define route(httpMethod, url, controllerType, method) \
    registerRoute(httpMethod, url, new RequestHandler<controllerType, StreamResponse>(this, &controllerType::method ));

#define route_type(httpMethod, url, controllerType, method, responseType) \
    registerRoute(httpMethod, url, new RequestHandler<controllerType, responseType>(this, &controllerType::method ));

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

			API virtual void registerRoute(string httpMethod, string route, RequestHandlerBase *handler);
			API virtual void setup();

			API virtual Response *serverInternalError(string message);
			API virtual bool handles(string method, string url);
			API vector<string> getUrls();

		protected:
			Server *server;
			string prefix;
			map<string, RequestHandlerBase* > routes;
			vector<string> urls;
		};

		class HTTPHandler : public Handler {
		public:
			API HTTPHandler();
			API void preProcess(Request *request, Response *response);
		};
	}
}
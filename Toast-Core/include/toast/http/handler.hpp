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
    register_route(httpMethod, url, new Toast::HTTP::RequestHandler<handlerType, Toast::HTTP::StreamResponse>(this, &handlerType::method ));

#define route_type(httpMethod, url, handlerType, method, responseType) \
    register_route(httpMethod, url, new Toast::HTTP::RequestHandler<handlerType, responseType>(this, &handlerType::method ));

namespace Toast {
	namespace HTTP {
		class Server;	// Fwd Declaration for Toast::HTTP::Server

		class Handler {
		public:
			API Handler();
			API virtual ~Handler();

			API virtual void set_server(Server *server);
			API virtual void pre_process(Request *request, Response *response);
			API virtual Response *process(Request *request);
			API virtual void post_process(Request *request, Response *response);

			API virtual Response *handle_request(Request *request);
			API void set_prefix(string prefix);

			API virtual void webSocketReady(WebSocket *websocket);
			API virtual void webSocketData(WebSocket *websocket, string data);
			API virtual void webSocketClosed(WebSocket *websocket);
			API virtual void register_web_socket(string route, WebSocketHandler *handler);
			API virtual void web_socket(string route, WebSocketHandler *handler);	// same as registerWebSocket

			API virtual void register_route(string httpMethod, string route, RequestHandlerBase *handler);
			API virtual void setup();

			API virtual Response *serverInternalError(string message);
			API virtual bool handles(string method, string url);
			API vector<string> get_urls();

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
			API void pre_process(Request *request, Response *response);
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
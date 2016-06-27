#pragma once

#include "toast/library.hpp"
#include "toast/http/request.hpp"
#include "toast/http/response.hpp"
#include "toast/http/websocket.hpp"
#include "toast/http/handler.hpp"
#include "toast/concurrent/mutex.hpp"
#include "mongoose.h"

using namespace Toast::Concurrent;

namespace Toast {
	namespace HTTP {
		API string htmlEntities(string data);

		class Server {
		public:
			API Server(int port);
			API ~Server();
			API void start(int pollTiming = 500);
			API void stop();
			API void registerHandler(Handler *handler);

			API Response *handleRequest(Request *request);
			API WebSocketContainer *getWebSockets();
			API bool handles(string method, string url);

			// Internal Handling
			API int _handleRequest(struct mg_connection *conn, struct http_message *msg);
			API void _webSocketReady(struct mg_connection *conn, struct http_message *msg);
			API int _webSocketData(struct mg_connection *conn, string data);
			API void _webSocketClosed(struct mg_connection *conn);
		private:
			bool stopped;
			int port;
			Mutex mutex;
			map<struct mg_connection*, Request *> currentRequests;
			struct mg_mgr mgr;
			WebSocketContainer websockets;
			vector<Handler *> handlers;
		};
	}
}
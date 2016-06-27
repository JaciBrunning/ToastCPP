#pragma once

#include "toast/http/request.hpp"
#include "toast/concurrent/mutex.hpp"

#include <vector>
#include <iostream>
#include <mongoose.h>

using namespace std;
using namespace Toast::Concurrent;

enum {
	WEBSOCKET_OPCODE_CONTINUATION = 0x0,
	WEBSOCKET_OPCODE_TEXT = 0x1,
	WEBSOCKET_OPCODE_BINARY = 0x2,
	WEBSOCKET_OPCODE_CONNECTION_CLOSE = 0x8,
	WEBSOCKET_OPCODE_PING = 0x9,
	WEBSOCKET_OPCODE_PONG = 0xa,
	WEBSOCKET_FIN = 0x80
};

namespace Toast {
	namespace HTTP {

		class WebSocketContainer;

		class WebSocket {
		public:
			API WebSocket(struct mg_connection *connection, struct http_message *msg);
			API Request *request();
			API struct mg_connection *getConnection();
			
			API void send(string data, int opcode = WEBSOCKET_OPCODE_TEXT);
			API void appendData(string data);
			API string flushData();

			API void close();
			API bool isClosed();

			API void setId(int id_);
			API int getId();

			API void addContainer(WebSocketContainer *wsc);
			API void removeContainer(WebSocketContainer *wsc);
			API void notifyContainers();

		protected:
			Mutex mutex;
			int id;
			string data;
			struct mg_connection *connection;
			struct http_message *message;
			bool closed;
			vector<WebSocketContainer *> containers;
			Request req;
		};

		class WebSocketContainer {
		public:
			API WebSocketContainer();
			API virtual ~WebSocketContainer();

			API void add(WebSocket *ws);
			API void send(string data);
			API WebSocket *get(struct mg_connection *connection);
			API void clean();
			API void remove(WebSocket *ws, bool lock = true);
			API WebSocket *getById(int id);

		protected:
			Mutex mutex;
			map<struct mg_connection*, WebSocket*> websockets;
			map<int, WebSocket*> websocketsById;
			int id;
		};
	}
}
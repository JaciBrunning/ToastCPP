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
			API struct mg_connection *get_connection();
			
			API void send(string data, int opcode = WEBSOCKET_OPCODE_TEXT);
			API void send_raw(char *data, int length, int opcode = WEBSOCKET_OPCODE_TEXT);
			API void append_data(string data);
			API string flush_data();

			API void close();
			API bool is_closed();

			API void set_id(int id_);
			API int get_id();

			API void add_container(WebSocketContainer *wsc);
			API void remove_container(WebSocketContainer *wsc);
			API void notify_containers();

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
			API WebSocket *get_by_id(int id);

		protected:
			Mutex mutex;
			map<struct mg_connection*, WebSocket*> websockets;
			map<int, WebSocket*> websocketsById;
			int id;
		};

		class WebSocketHandler {
		public:
			API virtual void on_ready(WebSocket *ws) {}
			API virtual void on_message(WebSocket *ws, string data) { }
			API virtual void on_closed(WebSocket *ws) { }
		};
	}
}
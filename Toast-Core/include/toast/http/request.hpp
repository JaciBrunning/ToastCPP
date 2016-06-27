#pragma once

#include "toast/library.hpp"
#include "toast/http/response.hpp"
#include "mongoose.h"

#include <map>
#include <string>
#include <regex>

using namespace std;

namespace Toast {
	namespace HTTP {
		class Request {
		public:
			API Request(struct mg_connection *connection, struct http_message *msg);
			API void writeResponse(Response *response);

			API bool hasUriVariable(string key);
			API string getUriVariable(string key, string fallback = "");
			API string get(string key, string fallback = "");	// same as getUriVariable

			API bool hasPostVariable(string key);
			API string getPostVariable(string key, string fallback = "");
			API string post(string key, string fallback = "");	// same as getPostVariable

			API string getHeader(string key);
			API map<string, string> getAllHeaders();

			API smatch getMatches();
			API bool match(string pattern);

			API string getUrl();
			API string getMethod();
			API string getData();

		protected:
			string method;
			string url;
			string data;
			struct mg_connection *connection;
			struct http_message *message;

			smatch matches;
		};

		class RequestHandlerBase {
		public:
			virtual Response *process(Request *request) = 0;
		};

		template<typename T, typename R>
		class RequestHandler : public RequestHandlerBase {
		public:
			typedef void (T::*func)(Request *request, R *response);

			API RequestHandler(T *handler_, func function_) : handler(handler_), function(function_) { }

			API Response *process(Request *request) {
				R *response = new R;
				
				try {
					handler->preProcess(request, response);
					(handler->*function)(request, response);
				} catch (string exception) {
					return handler->serverInternalError(exception);
				} catch (...) {
					return handler->serverInternalError("Unknown error");
				}

				return response;
			}

		protected:
			T *handler;
			func function;
		};
	}
}
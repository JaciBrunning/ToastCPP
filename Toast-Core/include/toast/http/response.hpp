#pragma once

#include "toast/library.hpp"
#include "mongoose.h"

#include <map>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_FORBIDDEN 403
#define HTTP_INTERNAL_ERROR 500

namespace Toast {
	namespace HTTP {
		class Response {
		public:
			API Response();
			API virtual ~Response();

			API virtual bool hasHeader(string key);
			API virtual void setHeader(string key, string value);
			API virtual string getData();
			API virtual void setCode(int code);
			API virtual string getBody() = 0;	// This should be overridden

		protected:
			int code;
			map<string, string> headers;
		};

		class BasicResponse : public Response {
		public:
			API virtual void setBody(string content);
			API virtual string getBody();
		
		protected:
			string body;
		};

		class StreamResponse : public ostringstream, public Response {
		public:
			API virtual string getBody();
		};
	}
}
#pragma once

#include "toast/library.hpp"
#include "toast/http/template.hpp"
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
		std::string mime_type(std::string extension, std::string fallback);

		class Response {
		public:
			API Response();
			API virtual ~Response();

			API virtual bool has_header(string key);
			API virtual void set_header(string key, string value);
			API virtual string get_data();
			API virtual void set_code(int code);
			API virtual string get_body() = 0;	// This should be overridden

		protected:
			int code;
			map<string, string> headers;
		};

		class BasicResponse : public Response {
		public:
			API virtual void set_body(string content);
			API virtual string get_body();
		
		protected:
			string body;
		};

		class StreamResponse : public ostringstream, public Response {
		public:
			API virtual string get_body();
		};

		class TemplateResponse : public Response {
		public:
			API void render(Template::Context *ctx, std::string template_name);
			API virtual string get_body();
		private:
			Template::Context *_ctx;
			string _tmpl;
		};
	}
}
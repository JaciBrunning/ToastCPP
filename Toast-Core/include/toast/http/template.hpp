#pragma once

#include "toast/library.hpp"

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <exception>

namespace Toast {
	namespace HTTP {
		namespace Template {
			class ParseException : public std::runtime_error {
			public:
				ParseException(std::string why) : runtime_error(why) {}
				ParseException(std::string why, int line, int col) : runtime_error(why + " (at: " + std::to_string(line) + ":" + std::to_string(col) + ")") {}
			};

			class TemplateObj {
			public:
				API TemplateObj() {};
				std::string using_template;
				bool is_using = false;
				std::map<std::string, std::string> definitions;
				std::vector<std::pair<int, std::string> > components;	// 0 for Literal, 1 for Reference, 2 for Include
			};

			class Context : public std::map<std::string, std::string> {
			public:
				API void add_template(std::string name, std::string content);
				API void add_template_file(std::string name, std::string file);
				API void add_template_resource(std::string name, std::string module, std::string resource);
				API std::string render(std::string name);
				API std::map<std::string, std::string> &args();
				std::string _lookup(std::string variable);
				std::string _render(std::string name);
			private:
				std::map<std::string, TemplateObj> _templates;
				std::map<std::string, std::string> temp_def;
			};

			API void parse(std::string _source, TemplateObj *_template);
		}
	}
}
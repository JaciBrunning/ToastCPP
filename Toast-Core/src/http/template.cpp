#include "toast/http/template.hpp"

#include "toast/resources.hpp"

#include <stdlib.h>
#include <streambuf>
#include <fstream>
#include <sstream>

using namespace Toast::HTTP::Template;
using namespace Toast;
using namespace std;

#define GCTX_ADD(name, file) global_context.add_template_file(name, Resources::get_resource_file("Toast-Core", file))

static bool global_init = false;
static Context global_context;

static void init_global_ctx() {
	if (!global_init) {
		global_init = true;		// Prevents an indirect recursion from Template::parse()
		
		GCTX_ADD("core/include/css", "templates/include/css.html");
		GCTX_ADD("core/include/js", "templates/include/js.html");
		
		GCTX_ADD("core/include/header", "templates/include/header.html");

		GCTX_ADD("core/components/readout/motors", "templates/components/readout/motors.html");
		GCTX_ADD("core/components/readout/relay", "templates/components/readout/relay.html");
		GCTX_ADD("core/components/readout/analog_out", "templates/components/readout/analog_out.html");
		GCTX_ADD("core/components/readout/pcm", "templates/components/readout/pcm.html");
		GCTX_ADD("core/components/readout/dio", "templates/components/readout/dio.html");

		GCTX_ADD("core/base", "templates/base.html");
	}
}

class ScopedBuffer {
public:
	ScopedBuffer(int len) : _len(len) {
		_buffer = (char *)malloc(_len);
	}

	virtual ~ScopedBuffer() {
		free(_buffer);
	}

	char& operator[](int i) {
		return _buffer[i];
	}

	std::string as_string(int n) {
		return std::string(_buffer, n);
	}
private:
	char *_buffer;
	int _len;
};

static string trim(string str) {
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

void Toast::HTTP::Template::parse(std::string _source, TemplateObj *t) {
	init_global_ctx();	// This only runs once because of a guard function

	bool directives(true);

	int i = 0;
	int j = 0;
	int l = _source.length();
	char tmpbuf[128];
	int off = 0;
	int line = -1;
	int currentStart = -1;

	ScopedBuffer result(l);
	int result_idx = 0;

	while (directives && i < l) {
		if (_source[i] == '#') {
			while (_source[i] != ' ' && i < l) {
				tmpbuf[off] = _source[i];
				off++;
				i++;
			}
			string name = string(tmpbuf, off);
			off = 0;
			i++;
			while (_source[i] != '\n' && i < l) {
				tmpbuf[off] = _source[i];
				off++;
				i++;
			}
			line++;
			string directive = string(tmpbuf, off);
			off = 0;
			if (name == "#using") {
				t->using_template = directive;
				t->is_using = true;
			}
			else if (name == "#define") {
				int a = 0;
				while (directive[a] != ' ') {
					tmpbuf[off] = directive[a];
					off++;
					a++;
				}
				a++;
				string def_name = string(tmpbuf, off);
				off = 0;
				while (a < directive.length()) {
					tmpbuf[off] = directive[a];
					off++;
					a++;
				}
				string def_val = string(tmpbuf, off);
				off = 0;
				t->definitions[def_name] = def_val;
			}
			i++;
		}
		else directives = false;
	}

	for (; i < l; i += 2) {
		char c = _source[i];
		char prev = '-';
		char next = '-';
		bool hasnext = (i != l - 1);

		bool run(true);

		if (i != 0)		prev = _source[i - 1];
		if (hasnext)	next = _source[i + 1];

		switch (c) {
		case '{':
			if (((prev == '{') && (j = -1)) ||
				((next == '{') && (j = 0) == 0)) {
				pair<int, string> pair1(0, result.as_string(result_idx));
				t->components.push_back(pair1);
				result_idx = 0;

				currentStart = i + j;
				while (run) {
					i = currentStart + 2 + off;
					if (i >= l) throw ParseException("Tag never closed!", line, currentStart);
					if (_source[i] == '}' && _source[i + 1] == '}') {
						run = false;
					}
					else {
						tmpbuf[off] = _source[i];
					}
					if (_source[i] == '\n') line++;
					off++;
				}
				string tag = trim(string(tmpbuf, off - 1));
				if (!tag.compare(0, 8, "include ")) {
					pair<int, string> pair2(2, tag.substr(8, tag.length() - 8));
					t->components.push_back(pair2);
				}
				else {
					pair<int, string> pair2(1, tag);
					t->components.push_back(pair2);
				}
				off = 0;
			}
			break;
		case '\n':
			line++;
			result[result_idx] = '\n';
			if (hasnext && next != '{') {
				result[result_idx + 1] = next;
				result_idx += 2;
			}
			else result_idx++;
			break;
		default:
			result[result_idx] = c;
			if (hasnext && next != '{') {
				result[result_idx + 1] = next;
				result_idx += 2;
			}
			else result_idx++;
			break;
		}
	}
	pair<bool, string> pair(0, result.as_string(result_idx));
	t->components.push_back(pair);
}

void Context::add_template(string name, string content) {
	TemplateObj t;
	parse(content, &t);
	_templates[name] = t;
}

void Context::add_template_file(string name, string file) {
	ifstream f(file);
	stringstream str_stream;
	str_stream << f.rdbuf();
	add_template(name, str_stream.str());
	f.close();
}

string Context::render(string name) {
	string resp = _render(name);
	temp_def.clear();
	return resp;
}

std::map<string, string> &Context::args() {
	return temp_def;
}

string Context::_lookup(string var) {
	auto it = find(var);
	if (it != end()) return it->second;
	else {
		it = temp_def.find(var);
		if (it != temp_def.end()) return it->second;
	}
	return "{{" + var + "}}";
}

string Context::_render(string name) {
	string _buf;
	TemplateObj *t;
	Context *ctx;
	if (_templates.find(name) != _templates.end()) {
		ctx = this;
		t = &_templates[name];
	} else if (global_context._templates.find(name) != global_context._templates.end()) {
		ctx = &global_context;
		t = &global_context._templates[name];
	} else {
		return "";
	}

	temp_def.insert(t->definitions.begin(), t->definitions.end());

	for (auto i : t->components) {
		if (i.first == 0) {
			_buf += i.second;
		}
		else if (i.first == 1) {
			_buf += _lookup(i.second);
		}
		else if (i.first == 2) {
			_buf += _render(i.second);
		}
	}

	if (t->is_using) {
		temp_def["#"] = _buf;
		return _render(t->using_template);
	}
	return _buf;
}
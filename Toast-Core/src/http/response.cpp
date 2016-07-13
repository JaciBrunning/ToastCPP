#include "toast/http/request.hpp"
#include "toast/http/response.hpp"

#include <map>

using namespace Toast::HTTP;
using namespace std;

static map<string, string> MIME {
	{"html", "text/html"},
	{"html", "text/html"},
	{"htm", "text/html"},
	{"shtm", "text/html"},
	{"shtml", "text/html"},
	{"css", "text/css"},
	{"js", "application/x-javascript"},
	{"ico", "image/x-icon"},
	{"gif", "image/gif"},
	{"jpg", "image/jpeg"},
	{"jpeg", "image/jpeg"},
	{"png", "image/png"},
	{"svg", "image/svg+xml"},
	{"txt", "text/plain"},
	{"torrent", "application/x-bittorrent"},
	{"wav", "audio/x-wav"},
	{"mp3", "audio/x-mp3"},
	{"mid", "audio/mid"},
	{"m3u", "audio/x-mpegurl"},
	{"ogg", "application/ogg"},
	{"ram", "audio/x-pn-realaudio"},
	{"xml", "text/xml"},
	{"ttf", "application/x-font-ttf"},
	{"json", "application/json"},
	{"xslt", "application/xml"},
	{"xsl", "application/xml"},
	{"ra", "audio/x-pn-realaudio"},
	{"doc", "application/msword"},
	{"exe", "application/octet-stream"},
	{"zip", "application/x-zip-compressed"},
	{"xls", "application/excel"},
	{"tgz", "application/x-tar-gz"},
	{"tar", "application/x-tar"},
	{"gz", "application/x-gunzip"},
	{"arj", "application/x-arj-compressed"},
	{"rar", "application/x-rar-compressed"},
	{"rtf", "application/rtf"},
	{"pdf", "application/pdf"},
	{"swf", "application/x-shockwave-flash"},
	{"mpg", "video/mpeg"},
	{"webm", "video/webm"},
	{"mpeg", "video/mpeg"},
	{"mov", "video/quicktime"},
	{"mp4", "video/mp4"},
	{"m4v", "video/x-m4v"},
	{"asf", "video/x-ms-asf"},
	{"avi", "video/x-msvideo"},
	{"bmp", "image/bmp"}
};

string Toast::HTTP::mime_type(string ext, string fallback) {
	if (MIME.find(ext) == MIME.end()) return fallback;
	return MIME[ext];
}

Response::Response() : code(HTTP_OK), headers() { }
Response::~Response() { }

bool Response::hasHeader(string key) {
	return headers.find(key) != headers.end();
}

void Response::setHeader(string key, string value) {
	headers[key] = value;
}

string Response::getData() {
	string body = getBody();
	ostringstream data;

	data << "HTTP/1.0 " << code << "\r\n";

	if (!hasHeader("Content-Length")) {
		ostringstream length;
		length << body.size();
		setHeader("Content-Length", length.str());
	}

	map<string, string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++) {
		data << (*it).first << ": " << (*it).second << "\r\n";
	}

	data << "\r\n";

	data << body;

	return data.str();
}

void Response::setCode(int code_) {
	code = code_;
}

// Basic

void BasicResponse::setBody(string content) {
	body = content;
}

string BasicResponse::getBody() {
	return body;
}

// Stream
string StreamResponse::getBody() {
	return this->str();
}

// Template
void TemplateResponse::render(Template::Context *ctx, string tmpl_name) {
	_ctx = ctx;
	_tmpl = tmpl_name;
}

string TemplateResponse::getBody() {
	return _ctx->render(_tmpl);
}
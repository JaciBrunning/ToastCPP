#include "toast/http/request.hpp"
#include "toast/http/response.hpp"

using namespace Toast::HTTP;

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
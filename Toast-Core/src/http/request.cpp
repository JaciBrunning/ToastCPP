#include "toast/http/request.hpp"

using namespace Toast::HTTP;

static char not_used[10];

Request::Request(struct mg_connection *conn, struct http_message *msg) : connection(conn), message(msg), matches() {
	url = string(message->uri.p, message->uri.len);
	method = string(message->method.p, message->method.len);
	data = string(message->body.p, message->body.len);
}

string Request::get_url() {
	return url;
}

string Request::get_method() {
	return method;
}

string Request::get_data() {
	return data;
}

void Request::write_response(Response *response) {
	string data = response->get_data();
	mg_send(connection, data.c_str(), data.size());
	//mg_send_http_chunk(connection, "", 0);
}

bool Request::has_uri_variable(string key) {
	if (mg_get_http_var(&message->query_string, key.c_str(), not_used, 1) != 0) {
		return true;
	}
	return false;
}

string Request::get_uri_variable(string key, string fallback) {
	char buffer[1024];
	int ret = mg_get_http_var(&message->query_string, key.c_str(), buffer, 1024);
	if (ret == 0 || ret == -1) {
		return fallback;
	}
	return string(buffer);
}

string Request::get(string key, string fallback) {
	return get_uri_variable(key, fallback);
}

bool Request::has_post_variable(string key) {
	if (mg_get_http_var(&message->body, key.c_str(), not_used, 1) != 0) {
		return true;
	}
	return false;
}

string Request::get_post_variable(string key, string fallback) {
	char buffer[1024];
	int ret = mg_get_http_var(&message->body, key.c_str(), buffer, 1024);
	if (ret == 0 || ret == -1) {
		return fallback;
	}
	return string(buffer);
}

string Request::post(string key, string fallback) {
	return get_post_variable(key, fallback);
}

string Request::get_header(string key) {
	mg_str *str = mg_get_http_header(message, key.c_str());
	if (str == NULL) return string();
	return string(str->p, str->len);
}

map<string, string> Request::get_all_headers() {
	map<string, string> map;
	int i;
	for (i = 0; i < MG_MAX_HTTP_HEADERS && message->header_names[i].len > 0; i++) {
		struct mg_str hn = message->header_names[i];
		struct mg_str hv = message->header_values[i];
		map[string(hn.p, hn.len)] = string(hv.p, hv.len);
	}
	return map;
}

smatch Request::get_matches() {
	return matches;
}

bool Request::match(string pattern) {
	string key = method + ":" + url;
	return regex_match(key, matches, regex(pattern));
}
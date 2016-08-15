#include "toast/bootstrap/server.hpp"
#include "toast/bootstrap.hpp"
#include "toast/resources.hpp"
#include "toast/filesystem.hpp"

using namespace Toast::Bootstrap;
using namespace Toast;

static HTTP::Template::Context ctx;

class BootstrapHTTPHandler : public HTTP::HTTPHandler {
public:
	void index(HTTP::Request *req, HTTP::TemplateResponse *resp) {
		resp->render(&ctx, "index");
	}

	void template_response(HTTP::Request *req, HTTP::TemplateResponse *resp) {
		resp->render(&ctx, req->get_url().substr(1));
	}

	void set_config(HTTP::Request *req, HTTP::StreamResponse *resp) {
		std::string cfg_name = req->get_matches()[1];

		std::string f = Filesystem::path("config/" + cfg_name + ".json");
		ofstream file_out;
		file_out.open(f);
		file_out << req->get_data() << endl;
		file_out.close();
	}

	void setup() {
		route_type("GET", "/", BootstrapHTTPHandler, index, HTTP::TemplateResponse);
		route_type("GET", "/readouts", BootstrapHTTPHandler, template_response, HTTP::TemplateResponse);
		route_type("GET", "/config", BootstrapHTTPHandler, template_response, HTTP::TemplateResponse);
		route("POST", "/api/bootstrap/setconfig/(.*)", BootstrapHTTPHandler, set_config);
	}
};

static HTTP::Server *_server;
static BootstrapHTTPHandler _handler;
static std::thread _thread;

static void _start_func() {
	_server->start(1000 / get_config()->get_int("http.poll_frequency", 30));
}

void Web::prepare() {
	ctx.add_template_file("index", Resources::get_resource_file("Toast-Bootstrap", "index.html"));
	ctx.add_template_file("readouts", Resources::get_resource_file("Toast-Bootstrap", "readouts.html"));
	ctx.add_template_file("config", Resources::get_resource_file("Toast-Bootstrap", "config.html"));

	// Initial Config Settings
	Config *c = get_config();
	int port = c->get_int("http.port", 5801);
	c->get_int("http.update_frequency", 15);

	_server = new HTTP::Server(port);
	_server->register_handler(&_handler);
}

void Web::start() {
	_thread = std::thread(_start_func);
	_thread.detach();
}

void Web::stop() {
	_server->stop();
}

HTTP::Server *Web::get_server() {
	return _server;
}

std::thread *Web::get_server_thread() {
	return &_thread;
}
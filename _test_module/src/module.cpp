#include "toast.hpp"

#include "io/motor.hpp"
#include "toast/http/server.hpp"
#include "toast/http/template.hpp"
#include "toast/resources.hpp"

#include <iostream>
#include <thread>

using namespace Toast;
using namespace Toast::HTTP;
using namespace std;

using namespace IO;

Template::Context ctx;

class MyHttpHandler : public HTTPHandler {
public:
	void hello(Request *req, StreamResponse *resp) {
		*resp << "Hello: " << req->get_url() << endl;
	}

	void some_template(Request *req, TemplateResponse *resp) {
		ctx.args()["get"] = req->get("val");
		resp->render(&ctx, req->get_url().substr(1));
	}

	void index(Request *req, TemplateResponse *resp) {
		resp->render(&ctx, "index");
	}

	void setup() {
		route("GET", "/hello", MyHttpHandler, hello);
		route_type("GET", "/", MyHttpHandler, index, TemplateResponse);
	}
};

class MyModule : public Module {
    public:
        virtual void construct() {
			Talon t(1);
			t.set(0.5);

			Victor v(1);

            ofstream outfile("shared.txt", ios::binary);
			outfile.write(Memory::Shared::get(), TOAST_SHARED_MEMPOOL_SIZE);

			ctx["test_str"] = "Hello World";

			ctx.add_template("out", "#using base\n#define TITLE myTitle\nHello: {{ test_str }}");
			ctx.add_template("base", "<html><title>{{TITLE}}</title>{{include core/include/css}}</html>\n{{#}}\nGET => {{get}}");
			ctx.add_template_file("index", Resources::get_resource_file("_test_module", "index.html"));

			Server s(8001);
			MyHttpHandler h;
			s.register_handler(&h);
			s.start();
        }
};

MODULE_CLASS("MyTestModule", false, MyModule);
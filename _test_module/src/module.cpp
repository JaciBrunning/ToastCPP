#include "toast.hpp"

#include "io/motor.hpp"
#include "io/relay.hpp"
#include "io/pneumatics.hpp"
#include "io/digital.hpp"
#include "io/analog.hpp"

#include "toast/http/server.hpp"
#include "toast/http/template.hpp"
#include "toast/http/websocket.hpp"
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
			t.set(0.47);

			Talon t2(2);
			t2.set(-0.88);

			Relay r(0);
			r.set_forward(true);
			r.set_reverse(false);

			Relay r2(1);
			r2.set_forward(false);
			r2.set_reverse(true);
			r2.set_reverse(false);

			PCM pcm(12);
			pcm.set_solenoid(0, true);
			pcm.set_solenoid(1, false);
			pcm.set_solenoid(3, true);
			pcm.set_solenoid(4, true);
			Memory::shared()->pneumatics(0)->set_comp_current(31.32);
			Memory::shared()->pneumatics(0)->set_solenoid_black(2, true);

			DIO dio(0);

			DIO doo(1, DIO::Mode::OUTPUT);
			doo.set(true);
			doo.set_pwm_enable(53.33);
			doo.set_pwm_rate(56.63);

			AnalogOutput ao(0);
			ao.set(4.613);

			Victor v(1);

            ofstream outfile("shared.txt", ios::binary);
			outfile.write(Memory::shared()->get_store(), Memory::SharedPool::SIZE);

			ctx["test_str"] = "Hello World";

			ctx.add_template("out", "#using base\n#define TITLE myTitle\nHello: {{ test_str }}");
			ctx.add_template("base", "<html><title>{{TITLE}}</title>{{include core/include/css}}</html>\n{{#}}\nGET => {{get}}");
			ctx.add_template_file("index", Resources::get_resource_file("_test_module", "index.html"));

			Server s(8001);
			s.enable_memory_socket();
			MyHttpHandler h;
			s.register_handler(&h);
			s.start();
        }
};

MODULE_CLASS("MyTestModule", false, MyModule);
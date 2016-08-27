#include "toast.hpp"
#include "toast/net/util.hpp"

#include "io/motor.hpp"
#include "io/relay.hpp"
#include "io/pneumatics.hpp"
#include "io/digital.hpp"
#include "io/analog.hpp"
#include "io/ds.hpp"

#include "toast/http/server.hpp"
#include "toast/http/template.hpp"
#include "toast/http/websocket.hpp"
#include "toast/resources.hpp"

#include "toast/math/buffer.hpp"

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

Logger l("test_module");
Joystick *joy;
Talon *talon;
PCM *pcm;

static void trackfunc(Toast::State old, Toast::State cur) {
	l << "State: " + cur.to_string();
}

static void tickfunc(Toast::State state) {
	talon->set(joy->get_raw_axis(0));
	pcm->set_solenoid(0, joy->get_raw_button(0));
}

class MyModule : public Module {
    public:
        virtual void construct() {
			Talon t(1);
			t.set(0.47);

			talon = new Talon(3);

			Talon t2(2);
			t2.set(-0.88);

			Relay r(0);
			r.set_forward(true);
			r.set_reverse(false);

			Relay r2(1);
			r2.set_forward(false);
			r2.set_reverse(true);
			r2.set_reverse(false);

			pcm = new PCM(12);

			DIO dio(0);

			DIO doo(1, DIO::Mode::OUTPUT);
			doo.set(true);
			doo.set_pwm_enable(53.33);
			doo.set_pwm_rate(56.63);

			AnalogOutput ao(0);
			ao.set(4.613);

			Victor v(1);

			joy = new Joystick(0);

            ofstream outfile("shared.txt", ios::binary);
			outfile.write(Memory::shared()->get_store(), Memory::SharedPool::SIZE);

			ctx["test_str"] = "Hello World";

			Config c("test_module");
			c.load();
			c.get_bool("is_me", true);

			States::register_tracker(&trackfunc);
			States::register_ticker(&tickfunc);

			for (int i = 0; i < Toast::Net::Util::get_adapter_count(); i++) {
				Toast::Net::Util::Adapter *a = Toast::Net::Util::get_adapter(i);
				l << a->name;
				l << "\t" + a->description;
				l << "\t" + a->ip_addr;
			}

//			Server s(8001);
//			MyHttpHandler h;
//			s.register_handler(&h);
//			s.start();
        }
};

MODULE_CLASS("MyTestModule", false, MyModule);
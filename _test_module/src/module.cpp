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

#include "toast/command.hpp"

#include "toast/math/buffer.hpp"
#include "toast/math/vector.hpp"

#include <iostream>
#include <thread>

using namespace Toast;
using namespace Toast::HTTP;
using namespace Toast::Command;
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
Talon *talon, *t6, *t7, *t8;
PCM *pcm;
bool lift = false;

static void trackfunc(Toast::State old, Toast::State cur) {
	l << "State: " + cur.to_string();
}

static void tickfunc(Toast::State state) {
	talon->set(joy->get_raw_axis(0));
	pcm->set_solenoid(0, joy->get_raw_button(0));

	Executor::instance()->tick();
}

struct LiftCommand : Toast::Command::Command {
	void start() {
		l << "Lifting...";
		lift = true;
		timeout(2000);
	}

	void periodic() {
		float perc = (float)time_passed() / (float)timeout();
		t8->set(-(2*perc - 1)*(2*perc - 1) + 1);
	}

	void stop() {
		l << "Lift Stopped!";
		lift = false;
	}

	bool can_start() {
		return !lift;
	}
};

struct DriveCommand : Toast::Command::Command {
	void periodic() {
		t6->set(-joy->get_raw_axis(1));
		t7->set(-joy->get_raw_axis(5));

		if (joy->get_raw_button(0) && !last_button) {
			exec()->push(new LiftCommand());
		}
		last_button = joy->get_raw_button(0);
	}

private:
	bool last_button = false;
};

using namespace Toast::Math;

class MyModule : public Module {
    public:
        virtual void construct() {
			t6 = new Talon(6);
			t7 = new Talon(7);
			t8 = new Talon(8);

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

			States::register_tracker(&trackfunc);
			States::register_ticker(&tickfunc);

			Executor::instance()->push(new DriveCommand());

//			Server s(8001);
//			MyHttpHandler h;
//			s.register_handler(&h);
//			s.start();
        }
};

MODULE_CLASS("MyTestModule", false, MyModule);
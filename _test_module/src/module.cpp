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

class MyHttpHandler : public HTTPHandler {
public:
	void hello(Request *req, StreamResponse *resp) {
		*resp << "Hello: " << req->getUrl() << endl;
	}

	void setup() {
		route("GET", "/hello", MyHttpHandler, hello);
	}
};

class MyModule : public Module {
    public:
        virtual void construct() {
			Talon t(1);
			t.set(0.5);

			Victor v(1);
			cout << v.get() << endl;
			cout << t.get_type() << endl;
			cout << v.get_type() << endl;

            ofstream outfile("shared.txt", ios::binary);
			outfile.write(Memory::Shared::get(), TOAST_SHARED_MEMPOOL_SIZE);

			Template::Context ctx;
			ctx["test_str"] = "Hello World";

			ctx.add_template("out", "#using base\n#define TITLE myTitle\nHello: {{ test_str }}");
			ctx.add_template("base", "<html><title>{{TITLE}}</title></html>\n{{#}}");

			Log::info("TestThing", ctx.render("out"));

			Server s(8001);
			MyHttpHandler h;
			DirHandler h2("res", "resources");
			s.registerHandler(&h);
			s.registerHandler(&h2);
			s.start();
        }
};

MODULE_CLASS("MyTestModule", false, MyModule);
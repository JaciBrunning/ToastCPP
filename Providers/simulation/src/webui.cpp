#include "toast/bootstrap/server.hpp"

#include "thp/sim/webui.hpp"
#include "thp/sim/sim_provider.hpp"

#include "toast/resources.hpp"
#include "toast/filesystem.hpp"

using namespace Toast;
using namespace Sim;

static HTTP::Template::Context ctx;

class SimulationHTTPHandler : public HTTP::HTTPHandler {
public:
	void index(HTTP::Request *req, HTTP::TemplateResponse *resp) {
		resp->render(&ctx, "index");
	}

	void template_response(HTTP::Request *req, HTTP::TemplateResponse *resp) {
		resp->render(&ctx, req->get_url().substr(1));
	}
};

static SimulationHTTPHandler _handler;

void Web::init() {
	Bootstrap::Web::get_server()->register_handler(&_handler);
	Bootstrap::Web::add_route("Simulation", "simulation/");
}
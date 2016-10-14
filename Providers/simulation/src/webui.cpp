#include "toast/bootstrap/server.hpp"
#include "toast/http/websocket.hpp"

#include "thp/sim/webui.hpp"
#include "thp/sim/sim_provider.hpp"

#include "toast/resources.hpp"
#include "toast/filesystem.hpp"
#include "toast/memory.hpp"
#include "toast/state.hpp"

#define STRINGIFY(x) #x

using namespace Toast;
using namespace Sim;

static HTTP::Template::Context ctx;

const char *index_html =
#include "resources/index.html"
;

const char *simulation_js =
#include "resources/simulation.js"
;

const char *style_css =
#include "resources/style.css"
;

static std::vector<HTTP::WebSocket *> sockets;

class SHMSocket : public HTTP::WebSocketHandler {
public:
	void on_ready(HTTP::WebSocket *ws) {
		sockets.push_back(ws);
		char data[] = { (char)0x02, (char)Toast::States::current_robotstate() };
		ws->send_raw(&data[0], 2, WEBSOCKET_OPCODE_BINARY);
	}

	void on_message(HTTP::WebSocket *ws, string data) {
		char *b = (char *)data.c_str();
		uint8_t mode = (int8_t)b[0];

		if (mode == 0x01) {					// Joystick Update
			uint8_t id = (uint8_t)b[1];

			Toast::Memory::Shared::DS::Joystick *joy = Toast::Memory::shared()->joystick(id);
			MTX_LOCK(Toast::Memory::shared_mutex()->joy, id);
			memcpy(joy->get_store(), b + 2, joy->SIZE);
			MTX_UNLOCK(Toast::Memory::shared_mutex()->joy, id);
		} else if (mode == 0x02) {
			Toast::RobotState rs = (Toast::RobotState)(uint8_t)b[1];
			
			if (Toast::States::current_robotstate() != rs)
				Toast::States::Internal::set_state(rs);
		}
	}

	void on_closed(HTTP::WebSocket *ws) {
		sockets.erase(std::remove(sockets.begin(), sockets.end(), ws));
	}
};
static SHMSocket _wsocket;

class SimulationHTTPHandler : public HTTP::HTTPHandler {
public:
	void index(HTTP::Request *req, HTTP::TemplateResponse *resp) {
		resp->render(&ctx, "index");
	}

	void sim_js(HTTP::Request *req, HTTP::StreamResponse *resp) {
		resp->set_header("Content-Type", "text/javascript");
		*resp << (char *)simulation_js;
	}

	void style(HTTP::Request *req, HTTP::StreamResponse *resp) {
		resp->set_header("Content-Type", "text/css");
		*resp << (char *)style_css;
	}

	void setup() {
		route_type("GET", "/simulation", SimulationHTTPHandler, index, HTTP::TemplateResponse);
		route("GET", "/simulation/sim.js", SimulationHTTPHandler, sim_js);
		route("GET", "/simulation/style.css", SimulationHTTPHandler, style);
		register_web_socket("/simulation/socket/data", &_wsocket);
	}
};

static SimulationHTTPHandler _handler;

static void _trackfunc(Toast::State oldstate, Toast::State newstate) {
	for (auto ws : sockets) {
		char data[] = { (char)0x02, (char)(newstate.robot_state()) };
		ws->send_raw(&data[0], 2, WEBSOCKET_OPCODE_BINARY);
	}
}

void Web::init() {
	ctx.add_template("index", index_html);

	Bootstrap::Web::get_server()->register_handler(&_handler);
	Bootstrap::Web::add_route("Simulation", "simulation");

	Toast::States::register_tracker(_trackfunc);
}
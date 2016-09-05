#pragma once

#include "toast/http/server.hpp"
#include <thread>

namespace Toast {
	namespace Bootstrap {
		namespace Web {
			void prepare();
			void start();
			void stop();
			API void add_route(std::string name, std::string dest);
			API HTTP::Server *get_server();
			API std::thread *get_server_thread();
			API HTTP::Template::Context *get_context();
		}
	}
}
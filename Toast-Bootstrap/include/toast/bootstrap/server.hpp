#pragma once

#include "toast/http/server.hpp"
#include <thread>

namespace Toast {
	namespace Bootstrap {
		namespace Web {
			void prepare();
			void start();
			void stop();
			API HTTP::Server *get_server();
			API std::thread *get_server_thread();
		}
	}
}
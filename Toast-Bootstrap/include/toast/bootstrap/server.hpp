#pragma once

#include "toast/http/server.hpp"

namespace Toast {
	namespace Bootstrap {
		namespace Web {
			void prepare();
			void start();
			API HTTP::Server *get_server();
		}
	}
}
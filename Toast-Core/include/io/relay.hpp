#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"
#include "io/common.hpp"

namespace IO {
	API char *get_relay_block(int port);

	class Relay {
	public:
		API Relay(int port);
		API virtual ~Relay() = default;
	
		API int get_port();

		API void set_forward(bool on);
		API bool get_forward();

		API void set_reverse(bool on);
		API bool get_reverse();
	private:
		int _port;
		char *_shm;
	};
}
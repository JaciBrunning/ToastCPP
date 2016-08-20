#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"

#include "io/common.hpp"

namespace IO {
	class Relay {
	public:
		API Relay(int port);
		API virtual ~Relay() = default;
	
		API bool operator==(Relay &r2) const {
			return r2._port == _port;
		}

		API int get_port();

		API void set_forward(bool on);
		API bool get_forward();

		API void set_reverse(bool on);
		API bool get_reverse();
	private:
		int _port;
		Toast::Memory::Shared::IO::Relay *_mem;
	};
}
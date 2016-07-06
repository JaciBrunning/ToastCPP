#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"
#include "io/common.hpp"

#include <inttypes.h>

namespace Relay {
	API char *getBlockFor(int id);

	API void init(int port);
	API void set_forward(int port, bool on);
	API bool get_forward(int port);
	API void set_reverse(int port, bool on);
	API bool get_reverse(int port);
}
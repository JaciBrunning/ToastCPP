#pragma once

#include "io/accel.hpp"
#include "io/analog.hpp"
#include "io/digital.hpp"
#include "io/ds.hpp"
#include "io/I2C.hpp"
#include "io/motor.hpp"
#include "io/pneumatics.hpp"
#include "io/power.hpp"
#include "io/relay.hpp"
#include "io/SPI.hpp"

namespace IO {
	const std::string IPC_HANDLE_UPDATE = "io.update";
	API void update_now(bool wait = true);
}
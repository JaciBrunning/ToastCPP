#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"
#include "io/common.hpp"

namespace IO {
	API char *get_onboard_accel_block();
	
	class Accelerometer {
	public:
		virtual ~Accelerometer() = default;
		enum Range { 
			Range_2G = 0x00, 
			Range_4G = 0x01, 
			Range_8G = 0x02,
			Range_16G = 0x03 
		};

		virtual void set_range(Range range) = 0;
		virtual float x() = 0;
		virtual float y() = 0;
		virtual float z() = 0;
	};

	class BuiltInAccelerometer : public Accelerometer {
	public:
		BuiltInAccelerometer();

		virtual void set_range(Range range);
		virtual float x();
		virtual float y();
		virtual float z();
	};
}
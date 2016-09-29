#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"

namespace IO {
	class Accelerometer {
	public:
		API virtual ~Accelerometer() = default;
		typedef Toast::Memory::Shared::IO::AccelRange Range;

		API virtual void set_range(Range range) = 0;
		API virtual float x() = 0;
		API virtual float y() = 0;
		API virtual float z() = 0;
	};

	class BuiltInAccelerometer : public Accelerometer {
	public:
		API BuiltInAccelerometer();

		API void set_range(Range range) override;
		API float x() override;
		API float y() override;
		API float z() override;
	};
}
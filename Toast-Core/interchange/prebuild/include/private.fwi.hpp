#pragma once

#include "fwi.hpp"
namespace Toast {
	namespace Memory {
		enum class Verification {
			VERIFY_AWAIT = 100,
			VERIFY_CONFIRM = 101,
			VERIFY_DENIED = 102,
			VERIFY_CORRUPT_LIBRARY = 110,
			VERIFY_INFO_METHOD_MISSING = 111,
			VERIFY_ALLOC_METHOD_MISSING = 112,
			VERIFY_VALID = 120
		};
		struct PrivatePool;
	}
}

namespace Toast {
	namespace Memory {
		struct PrivatePool : public FWI::Block {
			static const int SIZE = 307;

			FWIFUNC uint8_t get_endian();
			FWIFUNC void set_endian(uint8_t value);

			FWIFUNC Verification get_verification();
			FWIFUNC void set_verification(Verification value);

			FWIFUNC bool get_restart();
			FWIFUNC void set_restart(bool value);

			FWIFUNC bool get_info_set();
			FWIFUNC void set_info_set(bool value);

			FWIFUNC char * get_module_name();
			FWIFUNC int module_name_length();

			FWIFUNC char * get_module_filepath();
			FWIFUNC int module_filepath_length();


		}; // struct: PrivatePool
	} // namespace: Memory
} // namespace: Toast

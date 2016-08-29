#include "private.fwi.hpp"

FWIFUNC uint8_t Toast::Memory::PrivatePool::get_endian() {
	return FWI_MEM_VAL(uint8_t, _store, 1);
}
FWIFUNC void Toast::Memory::PrivatePool::set_endian(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 1) = value;
}

FWIFUNC Toast::Memory::Verification Toast::Memory::PrivatePool::get_verification() {
	return (Toast::Memory::Verification)(_store[2]);
}
FWIFUNC void Toast::Memory::PrivatePool::set_verification(Toast::Memory::Verification value) {
	_store[2] = (char)value;
}

FWIFUNC bool Toast::Memory::PrivatePool::get_restart() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::PrivatePool::set_restart(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::PrivatePool::get_info_set() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::PrivatePool::set_info_set(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC char * Toast::Memory::PrivatePool::get_module_name() {
	return (_store + 3);
}
FWIFUNC int Toast::Memory::PrivatePool::module_name_length() {
	return 48;
}

FWIFUNC char * Toast::Memory::PrivatePool::get_module_filepath() {
	return (_store + 51);
}
FWIFUNC int Toast::Memory::PrivatePool::module_filepath_length() {
	return 256;
}



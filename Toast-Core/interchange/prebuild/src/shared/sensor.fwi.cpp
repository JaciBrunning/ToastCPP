#include "shared/sensor.fwi.hpp"

FWIFUNC bool Toast::Memory::Shared::IO::OnboardAccel::get_init() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::OnboardAccel::set_init(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::OnboardAccel::get_bootstrap() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::OnboardAccel::set_bootstrap(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC Toast::Memory::Shared::IO::AccelRange Toast::Memory::Shared::IO::OnboardAccel::get_range() {
	return (Toast::Memory::Shared::IO::AccelRange)(_store[1]);
}
FWIFUNC void Toast::Memory::Shared::IO::OnboardAccel::set_range(Toast::Memory::Shared::IO::AccelRange value) {
	_store[1] = (char)value;
}

FWIFUNC bool Toast::Memory::Shared::IO::OnboardAccel::get_range_update() {
	return FWI_IS_BIT_SET(_store[0], 2);
}
FWIFUNC void Toast::Memory::Shared::IO::OnboardAccel::set_range_update(bool value) {
	FWI_SET_BIT_TO(_store[0], 2, value ? 1 : 0);
}

FWIFUNC float Toast::Memory::Shared::IO::OnboardAccel::get_x() {
	return FWI_MEM_VAL(float, _store, 2);
}
FWIFUNC void Toast::Memory::Shared::IO::OnboardAccel::set_x(float value) {
	FWI_MEM_VAL(float, _store, 2) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::OnboardAccel::get_y() {
	return FWI_MEM_VAL(float, _store, 6);
}
FWIFUNC void Toast::Memory::Shared::IO::OnboardAccel::set_y(float value) {
	FWI_MEM_VAL(float, _store, 6) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::OnboardAccel::get_z() {
	return FWI_MEM_VAL(float, _store, 10);
}
FWIFUNC void Toast::Memory::Shared::IO::OnboardAccel::set_z(float value) {
	FWI_MEM_VAL(float, _store, 10) = value;
}



#include "toast/bootstrap/state.hpp"
#include "toast/provider.hpp"

using namespace Toast;

static void _periodic_func(bool disabled, bool auton, bool teleop, bool test) { }

static void _transition_func(bool disabled, bool auton, bool teleop, bool test) {
    State st = States::DISABLED();

    if (disabled)       st = States::DISABLED();
    else if (auton)     st = States::AUTO();
    else if (teleop)    st = States::TELEOP();
    else if (test)      st = States::TEST();

    States::Internal::set_state(st.robot_state());
}

void Bootstrap::States::init() {
//    thp_state_set_callback(&_periodic_func, &_transition_func);
	SYMBOL sym = Internal::Loader::get_symbol(*Bootstrap::get_provider(), "thp_state_set_callback");
	reinterpret_cast<void(*)(RawStateCallback, RawStateCallback)>(sym)(&_periodic_func, &_transition_func);
}

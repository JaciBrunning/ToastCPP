#include "toast/bootstrap/state.hpp"
#include "toast/bootstrap/io.hpp"

using namespace Toast;

static void _periodic_func(bool disabled, bool auton, bool teleop, bool test) {
    io_motors_periodic(disabled, auton, teleop, test);
	io_power_periodic(disabled, auton, teleop, test);
}

static void _transition_func(bool disabled, bool auton, bool teleop, bool test) {
    State st = States::DISABLED();

    if (disabled)       st = States::DISABLED();
    else if (auton)     st = States::AUTO();
    else if (teleop)    st = States::TELEOP();
    else if (test)      st = States::TEST();

    States::Internal::set_state(st.ordinal());
}

void Bootstrap::States::init() {
    thp_state_set_callback(&_periodic_func, &_transition_func);
}

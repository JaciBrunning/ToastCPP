#include "io/motor.hpp"
#include "toast/bootstrap/io.hpp"

void io_motors_periodic(bool disabled, bool auton, bool teleop, bool test) {
    bool en = !disabled;
    int id;
    for (id = 0; id < COUNT_SPD; id++) {
        // Each Speed Controller
        char *block = Motor::getBlockFor(id);
        int initflag = block[ADDR_SPD_BOOTINIT];
        int type = block[ADDR_SPD_TYPE];

        if (type != 0) {
            int port = block[ADDR_SPD_PORT];
            int m_interface = block[ADDR_SPD_INTERFACE];
            if (initflag == -1) {
                // thp_motor_init(port, m_interface, type);
                block[ADDR_SPD_BOOTINIT] = 1;
			} else if (initflag == 1) {
				float desired = *(float *)(block + ADDR_SPD_SET);
				// thp_motor_set(port, m_interface, desired);
				// *(float *)(block + ADDR_SPD_GET) = thp_motor_get(port, m_interface, desired);
			}
		}
    }
}
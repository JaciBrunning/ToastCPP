#pragma once

// TOAST-CORE
#include "toast/library.hpp"
#include "io/motor.hpp"

CAPI void thp_motor_init(int port, int m_interface, int type);
CAPI void thp_motor_set(int port, int m_interface, float speed);
CAPI float thp_motor_get(int port, int m_interface);

// CAN Speed Controller (SRX, Jaguar not supported) //
CAPI void thp_srx_set_p(int port, float p);     // X P_COEFFICIENT
CAPI void thp_srx_set_i(int port, float i);     // X I_COEFFICIENT
CAPI void thp_srx_set_d(int port, float i);     // X D_COEFFICIENT
CAPI float thp_srx_get_p(int port);             // X P_COEFFICIENT
CAPI float thp_srx_get_i(int port);             // X I_COEFFICIENT
CAPI float thp_srx_get_d(int port);             // X D_COEFFICIENT

CAPI float thp_srx_get_bus_voltage(int port);    // X V_BUS
CAPI float thp_srx_get_output_voltage(int port); // X V_OUT
CAPI float thp_srx_get_output_current(int port); // X CUR_OUT
CAPI float thp_srx_get_temperature(int port);    // X TEMP

CAPI float thp_srx_get_position(int port);                  // X POSITION 
CAPI float thp_srx_get_speed(int port);                     // X SPEED
CAPI bool thp_srx_get_limit_fwd_ok(int port);               // X FWD_LIM
CAPI bool thp_srx_is_fwd_limit_switch_closed(int port);     // X FWD_LIM
CAPI bool thp_srx_get_limit_rvs_ok(int port);               // X RVS_LIM
CAPI bool thp_srx_is_rvs_limit_switch_closed(int port);     // X RVS_LIM

CAPI int thp_srx_get_analog_in(int port);       // X ANALOG_IN
CAPI int thp_srx_get_analog_in_raw(int port);   // X ANALOG_IN_RAW
CAPI int thp_srx_get_analog_in_vel(int port);   // X ANALOG_IN_VEL
CAPI int thp_srx_get_enc_position(int port);    // X ENC_POSITION
CAPI int thp_srx_get_env_vel(int port);         // X ENC_VEL

CAPI int thp_srx_get_pin_state_quad_a(int port);    // X QUAD_A
CAPI int thp_srx_get_pin_state_quad_b(int port);    // X QUAD_B
CAPI int thp_srx_get_pin_state_quad_idx(int port);  // X QUAD_IDX
CAPI int thp_srx_get_quad_idx_rises(int port);      // X QUAD_IDX_RISES

CAPI int thp_srx_get_pulse_width_position(int port);    // X PULSE_WIDTH_POSITION
CAPI int thp_srx_get_pulse_width_vel(int port);         // X PULSE_WIDTH_VEL
CAPI int thp_srx_get_pulse_width_rtf(int port);         // X PULSE_WIDTH_RISE_TO_FALL
CAPI int thp_srx_get_pulse_width_rtr(int port);         // X PULSE_WIDTH_RISE_TO_RISE

CAPI void thp_srx_set_voltage_ramp_rate(int port, float rate);
CAPI void thp_srx_config_neutral_mode(int port, int neutralmode);
CAPI void thp_srx_encoder_tick_per_rev(int port, int codesPerRev);
CAPI void thp_srx_config_pot_turns(int port, int potTurns);
CAPI void thp_srx_config_soft_pos_limits(int port, float fwd, float rvs);

CAPI void thp_srx_disable_soft_pos_limits(int port);
CAPI void thp_srx_config_limit_mode(int port, int mode);
CAPI void thp_srx_config_fwd_limit(int port, float position);
CAPI void thp_srx_config_rvs_limit(int port, float position);

CAPI void thp_srx_config_max_output_voltage(int port, float volts);
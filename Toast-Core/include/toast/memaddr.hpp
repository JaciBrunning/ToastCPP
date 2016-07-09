// These are the addresses used in Shared Memory, including the Private and Shared memory 
// blocks. We put this in a separate hpp file simply because the memory.hpp file would
// become too cluttered. 

#pragma once

// Addresses to Use - Common
#define ADDR_ENDIAN     0x00

// START BLOCK PRIVATE
#define ADDR_VERIFY         0x01
#define ADDR_RESTART        0x02
#define ADDR_INFO_SET       0x03
#define ADDR_MOD_NAME       0x10
#define LEN_MOD_NAME        48
#define ADDR_MOD_FILE       0x100
#define LEN_MOD_FILE        256
// END PRIVATE

// START BLOCK SHARED
#define ADDR_BOOTSTRAP_PID  0x04    // Int (Len: 4)
#define ADDR_STATE_CURRENT  0x08
#define ADDR_STATE_LAST     0x09
#define ADDR_TICK_TIMING    0x0A
#define ADDR_DEBUG          0x0B
#define ADDR_MOD_ACT_STATE  0x10    // LENGTH = 128 (1 per module)

// Power And Distribution
#define ADDR_PWR_OFFSET			0x90
	#define ADDR_PWR_CAN_ID				0x00
	#define ADDR_PWR_PDP_VOLTAGE		0x04
	#define ADDR_PWR_PDP_TEMP			0x08
	#define ADDR_PWR_PDP_TOTAL_PWR		0x0C
	#define ADDR_PWR_PDP_TOTAL_ENG		0x10
	#define ADDR_PWR_CONTR_IN_VOLT		0x14
	#define ADDR_PWR_CONTR_IN_CUR		0x18
	#define ADDR_PWR_PDP_TOTAL_CUR		0x1C
	#define ADDR_PWR_PDP_CUR_OFFSET		0x20
	#define ADDR_PWR_CONTR_FAULTS_3V3	0x60
	#define ADDR_PWR_CONTR_FAULTS_5V	0x62
	#define ADDR_PWR_CONTR_FAULTS_6V	0x64
	#define ADDR_PWR_CONTR_BUS_ENABLE	0x66
	#define ADDR_PWR_IS_BROWNOUT		0x67
	#define ADDR_PWR_CONTR_VOLT_3V3		0x68
	#define ADDR_PWR_CONTR_VOLT_5V		0x6C
	#define ADDR_PWR_CONTR_VOLT_6V		0x70
	#define ADDR_PWR_CONTR_CUR_3V3		0x74
	#define ADDR_PWR_CONTR_CUR_5V		0x78
	#define ADDR_PWR_CONTR_CUR_6V		0x7C

// Digital IO
#define ADDR_DIO_OFFSET		0x110
#define LEN_DIO				0x10	// 16 Bytes per DIO Port
#define COUNT_DIO			26		// 26 Total DIO Ports (10 onboard, 16 MXP)
	#define ADDR_DIO_BOOTINIT			0x00
	#define ADDR_DIO_MODE				0x01
	#define ADDR_DIO_VALUE				0x02
	#define ADDR_DIO_PWM_PULSE_ENABLE	0x03
	#define ADDR_DIO_PULSE_LENGTH		0x04
	#define ADDR_DIO_PWM_DUTY_CYCLE		0x08
	#define ADDR_DIO_PWM_RATE			0x0C

// Relay
#define ADDR_RELAY_OFFSET	0x2B0
#define LEN_RELAY			4		// 4 Bytes per Relay Port
#define COUNT_RELAY			4		// 4 Total Relay Ports
	#define ADDR_RELAY_PORT			0x00
	#define ADDR_RELAY_BOOTINIT		0x01
	#define ADDR_RELAY_DIR_MASK		0x02

// Analog Static Settings
#define ADDR_AN_SS_OFFSET		0x0C
#define ADDR_AN_SS_SAMPLE_RATE	0x00

// Analog Out
#define ADDR_AN_OUT_OFFSET	0x2C0
#define LEN_AN_OUT			8		// 8 Bytes per Analog Out Port
#define COUNT_AN_OUT		2		// 2 Total Analog Out Ports
	#define ADDR_AN_OUT_PORT		0x00
	#define ADDR_AN_OUT_BOOTINIT	0x01
	#define ADDR_AN_OUT_VOLTAGE		0x04

// Analog In
#define ADDR_AN_IN_OFFSET	0x2D0
#define LEN_AN_IN			0x40	// 64 Bytes per Analog In Port
#define COUNT_AN_IN			8		// 8 Total Analog In Ports
	#define ADDR_AN_IN_PORT			0x00
	#define ADDR_AN_IN_BOOTINIT		0x01
	#define ADDR_AN_IN_VALUE		0x02
	#define ADDR_AN_IN_AVG_VAL		0x04
	#define ADDR_AN_IN_VOLTS		0x08
	#define ADDR_AN_IN_AVG_VOLTS	0x0C
	#define ADDR_AN_IN_AVG_BITS		0x10
	#define ADDR_AN_IN_OVSMPL_BITS	0x14	// Oversample Bits
	#define ADDR_AN_IN_LSB_WEIGHT	0x18
	#define ADDR_AN_IN_OFFSET_VAL	0x1C
	#define ADDR_AN_IN_ACCUM_MASK	0x20
	#define ADDR_AN_IN_ACCUM_COUNT	0x24
	#define ADDR_AN_IN_ACCUM_DB		0x28	// Accumulator Deadband
	#define ADDR_AN_IN_ACCUM_CENTRE	0x2C
	#define ADDR_AN_IN_ACCUM_VALUE	0x30
	#define ADDR_AN_IN_ACCUM_INIT	0x38	// Accumulator Set Initial

// Pneumatics, Solenoids and PCM
#define ADDR_PCM_OFFSET		0x4D0
#define LEN_PCM				0x10	// 16 Bytes per PCM
#define COUNT_PCM			2		// Up to 2 PCMs per Robot
	#define ADDR_PCM_CAN_ID					0x00
	#define ADDR_PCM_BOOTINIT				0x01
	#define ADDR_PCM_CMP_FAULT_STATUS		0x02	// Compressor Fault Bitmask
	#define ADDR_PCM_CONTROL_FLAGS			0x03
	#define ADDR_PCM_SOLENOID_GET			0x04	// Bitmask of each solenoid on the robot (1 bit per solenoid, boolean)
	#define ADDR_PCM_SOLENOID_SET			0x05	// Same as Solenoid_Get, but for setting instead
	#define ADDR_PCM_SOLENOID_BLACK			0x06	// Solenoid Blacklist bitmask (blacklist called on solenoid shorted)
	#define ADDR_PCM_SOLENOID_FAULT_STATUS	0x07
	#define ADDR_PCM_COMPRESSOR_CURRENT		0x08

// PWM (raw)
#define ADDR_PWM_OFFSET		0x4F0
#define LEN_PWM				0x30	// 48 Bytes per PWM Channel
#define COUNT_PWM			20		// 20 Total PWM ports (10 onboard, 10 MXP)	
	#define ADDR_PWM_PORT			0x00
	#define ADDR_PWM_BOOTINIT		0x01
	#define ADDR_PWM_PERIOD_MULT	0x02
	#define ADDR_PWM_CONTROL_FLAGS	0x03
	#define ADDR_PWM_RAW_SET		0x04
	#define ADDR_PWM_RAW_GET		0x06
	#define ADDR_PWM_POS_GET		0x08
	#define ADDR_PWM_POS_SET		0x0C
	#define ADDR_PWM_SPEED_GET		0x10
	#define ADDR_PWM_SPEED_SET		0x14
	#define ADDR_PWM_BOUNDS_MAX		0x18
	#define ADDR_PWM_BOUNDS_DB_MAX	0x1C	// Deadband Max
	#define ADDR_PWM_BOUNDS_CENTRE	0x20
	#define ADDR_PWM_BOUNDS_DB_MIN	0x24	// Deadband Min
	#define ADDR_PWM_BOUNDS_MIN		0x28
	#define ADDR_PWM_BOUNDS_MODE	0x2C

// Speed Controllers
#define ADDR_SPD_OFFSET     0x8B0
#define LEN_SPD             0x10    // 16 Bytes per Speed Controller
#define COUNT_SPD           0x10    // 16 Speed Controllers (16 * 16 = 256 Total Length)
    #define ADDR_SPD_ID				0x00
    #define ADDR_SPD_PORT			0x01
    #define ADDR_SPD_INTERFACE		0x02
    #define ADDR_SPD_TYPE			0x03
    #define ADDR_SPD_BOOTINIT		0x04
    #define ADDR_SPD_GET			0x08
    #define ADDR_SPD_SET			0x0C

// Servo
#define ADDR_SERVO_OFFSET	0x9B0
#define LEN_SERVO			0x70	// 112 Bytes long for all servos under one block
	#define ADDR_SERVO_BOOTINIT_MASK		0x00
	#define ADDR_SERVO_OFFLINE_PEND_MASK	0x08
	#define ADDR_SERVO_UPDATE_PEND_MASK		0x0C
	#define ADDR_SERVO_MIN_ANGLE			0x10
	#define ADDR_SERVO_MAX_ANGLE			0x14
	#define ADDR_SERVO_ANGLE_RANGE			0x18
	#define ADDR_SERVO_VALUE_OFFSET			0x20
	#define COUNT_SERVO_VALUE				20		// Count of PWM ports. These are all a 4 byte float

// On-Board Accelerometer
#define ADDR_OB_ACCEL_OFFSET	0xA20
	#define ADDR_OB_ACCEL_RANGE		0x00
	#define ADDR_OB_RANGE_UPDATE	0x01
	#define ADDR_OB_ACCEL_BOOTINIT	0x02
	#define ADDR_OB_ACCEL_X			0x04
	#define ADDR_OB_ACCEL_Y			0x08
	#define ADDR_OB_ACCEL_Z			0x0C

// Joysticks and Human Interface
#define ADDR_JOY_OFFSET			0xA30
#define LEN_JOY					0x30	// 48 Bytes per Joystick
#define COUNT_JOY				6		// Driver Station support 6 Joysticks
	#define ADDR_JOY_BOOTINIT		0x00
	#define ADDR_JOY_NUM_AXIS		0x01
	#define ADDR_JOY_NUM_BUTTON		0x02
	#define ADDR_JOY_NUM_POV		0x03
	#define ADDR_JOY_BUTTON_MASK	0x04
	#define ADDR_JOY_POV_OFFSET		0x08
	#define COUNT_JOY_POV			4		// Up to 4 POV controls on a single joystick seems reasonable
	#define ADDR_JOY_AXIS_OFFSET	0x10
	#define COUNT_JOY_AXIS			16		// Up to 16 Axis Controls on a single joystick is excessive, but reasonable
	#define ADDR_JOY_RUMBLE_L		0x20
	#define ADDR_JOY_RUMBLE_R		0x22
	#define ADDR_JOY_OUTPUTS		0x24	// These are miscellaneous, but WPILib supports them so I guess we will too

// Driver Station Information
#define ADDR_DS_INFO_OFFSET		0xB50
	#define ADDR_DS_INFO_DS_ATTACHED		0x00
	#define ADDR_DS_INFO_NEW_CTRL_DATA		0x01
	#define ADDR_DS_INFO_FMS_ATTACHED		0x02
	#define ADDR_DS_INFO_SYSTEM_ACTIVE		0x03	// This is basically whether the robot has been E-Stopped or in Disabled mode
	#define ADDR_DS_INFO_MATCH_TIME			0x04
	#define ADDR_DS_INFO_ALLIANCE			0x08
	#define ADDR_DS_INFO_ALLIANCE_STATION	0x09

// END SHARED
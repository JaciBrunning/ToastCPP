// These are the addresses used in Shared Memory, including the Private and Shared memory 
// blocks. We put this in a separate hpp file simply because the memory.hpp file would
// become too cluttered. 

#pragma once

// Addresses to Use - Common
#define ADDR_ENDIAN     0x00

// Addresses to Use - Private
#define ADDR_VERIFY         0x01
#define ADDR_RESTART        0x02
#define ADDR_INFO_SET       0x03
#define ADDR_MOD_NAME       0x10
#define LEN_MOD_NAME        48
#define ADDR_MOD_FILE       0x100
#define LEN_MOD_FILE        256

// Addresses to Use - Shared
#define ADDR_BOOTSTRAP_PID  0x04    // Int (Len: 4)
#define ADDR_STATE_CURRENT  0x08
#define ADDR_STATE_LAST     0x09
#define ADDR_TICK_TIMING    0x0A
#define ADDR_DEBUG          0x0B
#define ADDR_MOD_ACT_STATE  0x10    // LENGTH = 256 (1 per module)

// Power And Distribution
#define ADDR_PWR_OFFSET			0x120
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
	#define ADDR_PWR_CONTR_VOLT_3V3		0x68
	#define ADDR_PWR_CONTR_VOLT_5V		0x6C
	#define ADDR_PWR_CONTR_VOLT_6V		0x70
	#define ADDR_PWR_CONTR_CUR_3V3		0x74
	#define ADDR_PWR_CONTR_CUR_5V		0x78
	#define ADDR_PWR_CONTR_CUR_6V		0x7C

// Speed Controllers
#define ADDR_SPD_OFFSET     0x1C0
#define LEN_SPD             0x10    // 16 Bytes per Speed Controller
#define COUNT_SPD           0x10    // 16 Speed Controllers (16 * 16 = 256 Total Length)
    #define ADDR_SPD_ID         0x00
    #define ADDR_SPD_PORT       0x01
    #define ADDR_SPD_INTERFACE  0x02
    #define ADDR_SPD_TYPE       0x03
    #define ADDR_SPD_BT_INIT    0x04    // Boostrap Init
    #define ADDR_SPD_GET        0x08
    #define ADDR_SPD_SET        0x0C
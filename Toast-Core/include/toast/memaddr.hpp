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

// Speed Controllers
#define ADDR_SPD_OFFSET     0x130
#define LEN_SPD             0x10    // 16 Bytes per Speed Controller
#define COUNT_SPD           0x10    // 16 Speed Controllers (16 * 16 = 256 Total Length)
    #define ADDR_SPD_ID         0x00
    #define ADDR_SPD_PORT       0x01
    #define ADDR_SPD_INTERFACE  0x02
    #define ADDR_SPD_TYPE       0x03
    #define ADDR_SPD_BT_INIT    0x04    // Boostrap Init
    #define ADDR_SPD_GET        0x08
    #define ADDR_SPD_SET        0x0C
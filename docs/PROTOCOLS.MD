# Protocols
This document outlines each of the protocols Toast uses in its internals. Toast
C++ is designed to be fast, efficient and comprehensive, and as such, some custom
protocols needed to be introduced.

## The RobotLog Protocol (The Logger)
Toast stores log files as `.tlog` files. These files are not regular text, and follow 
our custom RobotLog Protcol. Because modules are run in individual processes, each module
has its own log file. This can get confusing, so the RobotLog protocol includes special
information to help piece all these log files together, and to produce more information.

Each log in the RobotLog protocol follows the same general layout:  

| Position | Type | Name | Description |
| -------- | ---- | ---- | ----------- |
| 0x00     | Char | Code | The type of log |
| 0x01 - 0x04 | Int | Time | The time of the log, in milliseconds since the epoch |
| extra info |||||

Currently, there is one code in the protocol:
- 0x00: Message

More codes are planned in the future for debugging information, but are
not required for the time being.

### The Message
The Message Code is used for regular logging of messages by the logger
for the user to review. In general, this is a mirror of what is seen in
the stdout, but with some extra information for viewing purposes.

| Position | Type | Name | Description |
| -------- | ---- | ---- | ----------- |
| 0x00     | Char | Code | The type of log |
| 0x01 - 0x04 | Int | Time | The time of the log, in milliseconds since the epoch |
| 0x05 - 0x08 | Int | MatchTime | The time left in the match, in milliseconds |
| 0x09 | Char | RobotState | The state of the robot (0 = DISABLED, 1 = AUTO, 2 = TELEOP, 3 = TEST) |
| 0x10 | Char | Flags | 8 Sequential Bits relating to flags on the message. 0b0000 00XY. If X is 1, the message is for debugging. If Y is 1, the message is an error |
| 0x1A - 0x1D | Int | Length | The length of the total message |
| 0x1E+ | Char[] | Message | The message itself |

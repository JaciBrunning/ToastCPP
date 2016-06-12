# Memory
This document outlines how each of the Toast Shared Memory Blocks is formatted. Toast uses Shared Memory
to communicate with modules, which are loaded in separate processes. All hardware information, as well as
module information and Toast-Specific information is stored in Shared Memory. This is an outline of
Toast's default Shared Memory Blocks.

In Toast, Shared Memory Blocks are often over-allocated to allow for future expansion. This is done so that
modules compiled in earlier versions of Toast don't have to be recompiled in order to work. We over-allocate
to allow room for future additions, as changing the size of the Shared Memory Block would cause all modules
and code relying on the Toast Core to be recompiled.

## Private Block
The Private Block is a block of shared memory 512 Bytes large. The Private Block
is given a unique identifier by a hash function of the Module File absolute path on
the filesystem. The Private Block is used for communication between a single module 
and the Toast Bootstrapper, relaying information about the Module and its load state.

| Address | Type | Name | Description |
| ------- | ---- | ---- | ----------- |
| 0x00    | Char | Verification Byte | The Verification Byte is used to keep track of a modules load process and error state. This byte is set to `0x7A` before the module is loaded, and the module checks against this. If the module detects the byte as `0x7A`, it sets it to `0x7B` and continues with initialization. If it doesn't match, it is set to `0x80` and the module stopped. If a module file cannot be loaded (i.e. is corrupt), this byte is set to `0x8A`. If the `_get_module_information` method is missing, it is set to `0x8B`. If the `_allocate_module_instance` method is missing, it is set to `0x8C`. If there are no issues during the initial loading of the module, it is set to `0x7C` and the module is marked as valid. |
| 0x01    | Char | Restart Byte | The restart byte simply describes whether the module is configured to be restartable or not. If the module specifies that it can be restarted (and it has loaded correctly), it will have a value of `0x01`, else, `0x00`. If there are any load errors as described in the `Verification Byte`, the module may not restart, as it will get stuck in a loop. |
| 0x02    | Char | Info Set | If this is set to `0x01`, the information of the module (module name / restartable) has been set in the memory. |
| 0x03 - 0x0F | _Unused_ | _Unused_ | _Unused_ |
| 0x10 - 0x41 | Char[] | Module Name | 50 Bytes are allocated to store the name of the module. This is used as the identifier for the module and is identical to the one provided in the `MODULE_CLASS` macro |
| 0x42 - 0xFF | _Unussed_ | _Unused_ | _Unused_ |
| 0x100 - 0x1FF | Char[] | Module File | The filename of the module |

## Shared Block
The Shared Block is a block of shared memory 1 Megabyte large. The Shared block
can be used by all modules, as well as the Toast Bootstrap itself. The Shared Block
is used to relay information about Robot State, Motor and I/O values, and almost 
everything that is used in cross-process communication. For this reason, the Shared
Block is very large in comparison to the Private Block.

| Address | Type | Name | Description |
| ------- | ---- | ---- | ----------- |
| 0x00 - 0x03    | Bootstrap PID | Int | The Process ID of the Toast Bootstrap. This is used to automatically kill module children when the bootstrap is killed |
| 0x04    | Char | Debug | If set the `0x01`, Toast was launched in debug mode. This gives more in-depth logger output for the Bootstrap and Module Processes |
| 0x05    | _Unused_ | _Unused_ | _Unused_ |
| 0x06    | Char | Current State | The current state of the Robot as defined in `toast/state.hpp` |
| 0x07    | Char | Last State    | The last state of the Robot as defined in `toast/state.hpp` |
| 0x08    | Char | State Tick Timing | Stores how long (in milliseconds) modules should wait inbetween state ticks. |
| 0x09 - 0x0F | _Unused_ | _Unused_ | _Unused_ | 
| 0x10 - 0x109 | Char | Module Activity State | This 256 Byte Large block of memory contains information about module activity state. In short, each module is given a location in this block of memory, respective of the order they were loaded. The shared memory with name `module_private_{x}` will be the private block of that module, where `{x}` is the index of the entry minus the starting index of the Module Activity State Block (`0x10`). If the value is `0x00`, this module has not been discovered or loaded. If it is `0x01`, the module has been discovered but not loaded. If it is `0x02`, the module is currently active. If it is `0x03`, the module has recently crashed and is awaiting a restart if available. If the restart is successful, this becomes `0x04`.|
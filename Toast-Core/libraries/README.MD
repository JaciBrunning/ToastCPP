# Libraries
The Library directory contains the source files for all the libraries that the
Toast-Core uses. Some of these libraries may be modified to work within Toast and/or
its target platforms. All Libraries are kept as close to their original form as
possible.

All library source files / header files are combined with the Toast Core to be used
by modules and the components of Toast itself. In this way, only libraries that permit
redistribution are used.

Each library compiled into the Toast Core maintains its original license. All modifications
are within the bounds of the license chosen by the maintainers of the library.

## Library List
| Name | Folder | Repo | Changes |
| json11 | json 11 | https://github.com/borisgontar/json11 | Functions modified to work cross-platform, API (__cdecl) flags added |
| Mongoose Web Server | mongoose | https://github.com/cesanta/mongoose | Slightly modified to work on newer and older versions of MSVC for Windows |
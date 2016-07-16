# Toast for C++

The C++ version of the Toast library for FRC.

Keep in mind that this readme is temporary, as is this repository location, and will be modified when this project is deemed stable and ready. Currently, this project is not ready for release

The C++ version of Toast loads modules much like the Java version, but instead loads them in a new system process. Using shared memory, the modules connect to the bootstrap process and exchange
information about the robot state, and all I/O. Simulation is done through a WebUI.

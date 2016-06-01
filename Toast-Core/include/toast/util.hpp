#pragma once

#include "toast/environment.hpp"
#include "toast/library.hpp"

#ifdef OS_WIN
    #include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
    #include <sys/time.h>
    #include <time.h>
#else
    #include <sys/time.h>
#endif

#ifndef OS_WIN
    #include <sys/types.h>
    #include <unistd.h>
    #include <signal.h>
#endif

#include <errno.h>
#include <string>
#include <sstream>
#include <vector>

// A collection of utilities that don't need to be in Toast's namespace

// Checks if a string ends with another specific string.
API bool ends_with(std::string const & value, std::string const & ending);

// C++ doesn't provide a universal, cross-platform way to perform a sleep
// call, so here's our own one.
API void sleep_ms(int milliseconds);

// Get the current system time since UNIX epoch in Milliseconds
API long current_time_millis();

// Get the Process ID of the current process
API int get_pid();

// Check if a process ID is alive
API bool is_process_alive(int pid);

API std::vector<std::string> split(const std::string &s, char delim);
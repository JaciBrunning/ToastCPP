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

#include <algorithm>
#include <cctype>

// A collection of utilities that don't need to be in Toast's namespace

// Checks if a string ends with another specific string.
API bool ends_with(std::string const & value, std::string const & ending);

// Checks if a string starts with another specific string.
API bool starts_with(std::string const & value, std::string const & starting);

// C++ doesn't provide a universal, cross-platform way to perform a sleep
// call, so here's our own one.
API void sleep_ms(int milliseconds);

// Get the current system time since UNIX epoch in Milliseconds
API long current_time_millis();

// Get the current CPU time in milliseconds (for process; multiple threads may cause this to be higher than wall clock speed)
API long current_cpu_time();

// Get the Process ID of the current process
API int get_pid();

// Check if a process ID is alive
API bool is_process_alive(int pid);

API std::vector<std::string> split(const std::string &s, char delim);

API inline std::string trim(const std::string &s) {
   auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
   auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
   return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}
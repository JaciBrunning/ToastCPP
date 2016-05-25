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
    #include <unistd.h>
#endif

// A collection of utilities that don't need to be in Toast's namespace

// Checks if a string ends with another specific string.
API inline bool ends_with(std::string const & value, std::string const & ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

// C++ doesn't provide a universal, cross-platform way to perform a sleep
// call, so here's our own one.
API inline void sleep_ms(int milliseconds) {
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}

// Get the current system time since UNIX epoch in Milliseconds
API inline long current_time_millis() {
#ifdef WIN32
    SYSTEMTIME time;
    GetSystemTime(&time);
    return (long)((time.wSecond * 1000) + time.wMilliseconds);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}
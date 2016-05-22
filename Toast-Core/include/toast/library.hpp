#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define API __declspec(dllexport)
#else
    #define API
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define CAPI extern "C" __declspec(dllexport)
#else
    #define CAPI extern "C"
#endif
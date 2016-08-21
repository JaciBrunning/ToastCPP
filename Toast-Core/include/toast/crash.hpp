#pragma once

#include "toast/library.hpp"

#include <string>
#include <string.h>
#include <vector>

#include <exception>
#include <signal.h>
#include <cstdlib>

#define CRASH_HANDLE_START  \
        try {                 \
    
#define CRASH_HANDLE_END \
        } catch(const char *e) { \
            Toast::Crash::on_known(e); \
        } catch (const std::exception &e) { \
            Toast::Crash::on_known(e);      \
        } catch (...) {                     \
            Toast::Crash::on_unknown();     \
        }                                   \

namespace Toast {
    namespace Crash {
        API void initialize();
        
        API void on_known(std::exception e);
        API void on_known(const char *e);
        API void on_unknown();
        API void on_signal(int sigid);
        
		API void shutdown();
        API void on_shutdown(void (*arg)());
        
        API void handle_exception(std::string type, std::string msg);
    }
}
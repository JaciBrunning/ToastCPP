#include "toast/crash.hpp"
#include "toast/logger.hpp"
#include "toast/splash.hpp"
#include "toast/environment.hpp"
#include "toast/memory.hpp"
#include "toast/module.hpp"

#ifdef OS_WIN
    #include "compat/win32/backtrace.hpp"
    #include <windows.h>
	#include <eh.h>
#else
    #include "compat/unix/backtrace.hpp"
    #include <typeinfo>
#endif

#include <exception>
#include <stdexcept>

using namespace Toast;

static Logger __logger("Toast-Crash");
static void (*shutdown_ptr)() = NULL;

static void shutdown() {
    if (shutdown_ptr != NULL) shutdown_ptr();
    // abort();
    exit(0);
}

static void catch_fatal_signal(int sig) {
    Crash::on_signal(sig);
}

static void catch_nonfatal_signal(int sig) {
	::shutdown();
}

static void catch_terminate() {
    std::exception_ptr curr = std::current_exception();
    if (curr != 0) {
		CRASH_HANDLE_START
        std::rethrow_exception(curr);
		CRASH_HANDLE_END
	}
#ifdef OS_WIN
	abort();
#endif
}

void Crash::initialize() {
    signal(SIGFPE, catch_fatal_signal);
    signal(SIGILL, catch_fatal_signal);
    signal(SIGSEGV, catch_fatal_signal);
	signal(SIGINT, catch_nonfatal_signal);
    #ifndef OS_WIN
        signal(SIGBUS, catch_fatal_signal);
        signal(SIGSYS, catch_fatal_signal);
    #endif
}

void Crash::on_known(std::exception e) {
    std::string type(typeid(e).name());
    std::string msg(e.what());
    
    Crash::handle_exception(type, msg);
}

void Crash::on_known(const char *e) {
    std::string type = "<string>";
    std::string msg(e);
    
    Crash::handle_exception(type, msg);
}

void Crash::on_unknown() {
    std::string type = "<unknown type>";
    std::string msg = "<no message>";
    
    Crash::handle_exception(type, msg);
}

void Crash::on_signal(int sigid) {
    std::string type = "<signal>";
    std::string msg;
    
    if (sigid == SIGFPE) msg = "SIGFPE (Fatal Arithmetic Error)";
    else if (sigid == SIGILL) msg = "SIGILL (Illegal Instruction)";
    else if (sigid == SIGSEGV) msg = "SIGSEGV (Segmentation Violation)";
    #ifndef OS_WIN
        else if (sigid == SIGBUS) msg = "SIGBUS (Invalid Dereference)";
        else if (sigid == SIGSYS) msg = "SIGSYS (Bad System Call)";
    #endif
    
    Crash::handle_exception(type, msg);
}

void Crash::shutdown() {
	::shutdown();
}

void Crash::on_shutdown(void (*arg)()) {
    shutdown_ptr = arg;
}

std::vector<std::string> _pending_backtrace;
bool using_pending_bt = false;

void Crash::handle_pending_unwind() {
	_pending_backtrace = backtrace_get(2);
	using_pending_bt = true;
}

void Crash::handle_exception(std::string type, std::string msg) {
    __logger.raw("\n**** CRASH LOG ****");
    __logger.raw(Splash::get_error_splash() + "\n");
    __logger.raw("Your robot has crashed. Following is a crash log and more details.");
    
    __logger.raw("\nException Info: ");
    __logger.raw("\tType: " + type);
    __logger.raw("\tMessage: " + msg);
    
    __logger.raw("\nBacktrace:");
    std::vector<std::string> backtrace = using_pending_bt ? _pending_backtrace : backtrace_get(2);
    for (auto i : backtrace) {
        __logger.raw("\t" + i);
    }
    
    __logger.raw("\nModules:");
    for (auto module : Toast::get_all_modules()) {
        __logger.raw("\t" + module.file);
        __logger.raw("\t\tName: " + module.name);
        __logger.raw("\t\tId: " + std::to_string(module.module_idx));
        __logger.raw("\t\tStatus: " + std::to_string((int)module.status) + 
                " (" + 
                    (module.status == Memory::ModuleActState::DISCOVERED ? "Discovered" :
                        (module.status == Memory::ModuleActState::ACTIVE ? "Loaded" : 
                            (module.status == Memory::ModuleActState::CRASHED ? "Crashed" : "Restarted"))) + 
                ")\n");
    }
    
    __logger.raw("\n*******************");
    shutdown();
}

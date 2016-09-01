#include "toast/internal/loader.hpp"
#include "toast/crash.hpp"

#include <string>
#include <string.h>

using namespace Toast;
using namespace Toast::Internal;
using namespace std;

int _argc;
char **_argv;
bool module = false;

static void load_func();

#ifdef OS_WIN
// When the stack starts unwinding (i.e. we've encountered a crash), we take a snapshot of the backtrace
static struct BacktraceOnUnwindHack {
	~BacktraceOnUnwindHack() {
		Crash::handle_pending_unwind();
	};
};

#include "Windows.h"
#include "DbgHelp.h"
#include <WinBase.h>
#pragma comment(lib, "dbghelp.lib") 

static void run() {
	if (!IsDebuggerPresent()) {
		CRASH_HANDLE_START
			BacktraceOnUnwindHack hack;
			load_func();
		CRASH_HANDLE_END
	} else {
		load_func();
	}
}
#else
static void run() {
	load_func();
}
#endif

static void load_func() {
	if (_argc > 1) {
		if (strcmp(_argv[1], "__TOAST_MODULE_LD_FILE") == 0) {
			module = true;
		}
	}
	if (!module) {
		DYNAMIC dyn = Loader::load_dynamic_library(Loader::library_name("toast_bootstrap"));
		SYMBOL sym = Loader::get_symbol(dyn, "init_toast_bootstrap");
		reinterpret_cast<void(*)(int, char*[])>(sym)(_argc, _argv);
	}
	else {
		DYNAMIC dyn = Loader::load_dynamic_library(Loader::library_name("toast_module"));
		SYMBOL sym = Loader::get_symbol(dyn, "init_toast_module");
		reinterpret_cast<void(*)(string, string, int)>(sym)(string(_argv[2]), string(_argv[3]), atoi(_argv[4]));
	}
}

int main(int argc, char *argv[]) {
    Loader::initialize();
	Crash::initialize();

	_argc = argc;
	_argv = argv;

	run();
    return 0;
}
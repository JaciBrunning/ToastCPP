#include "toast/net/socket.hpp"
#include "toast/bootstrap.hpp"
#include "toast/bootstrap/loader.hpp"
#include "toast/bootstrap/state.hpp"
#include "toast/bootstrap/server.hpp"
#include "toast/memory.hpp"
#include "toast/filesystem.hpp"
#include "toast/logger.hpp"
#include "toast/splash.hpp"
#include "toast/environment.hpp"
#include "toast/state.hpp"
#include "toast/util.hpp"
#include "toast/crash.hpp"
#include "toast/config.hpp"

#include "thp/provider.hpp"

using namespace Toast;

Logger _b_log("Toast");
Config _b_cfg("Toast-Bootstrap");

void bootstrap_shutdown() {
	_b_log << "Bootstrap Stopped. Freeing Resources.";
	Log::flush();
	Log::close();

	provider_free();
	Toast::Memory::free_memory(true);
	Toast::Bootstrap::Loader::free();
}

void init_toast_bootstrap(int argc, char *argv[]) {
	bool loop = true, load = true;
	for (int i = 0; i < argc; i++) {
		char *a = argv[i];
		if (strcmp(a, "--no-loop") == 0) loop = false;
		else if (strcmp(a, "--no-load") == 0) load = false;
	}

    long start_time = current_time_millis();
    ProviderInfo *info = provider_info();
    Crash::initialize();
	Crash::on_shutdown(bootstrap_shutdown);
    
    CRASH_HANDLE_START
    provider_preinit();
    
    Net::Socket::socket_init();
    Filesystem::initialize();
    Memory::initialize_bootstrap();
    
    Log::initialize("Bootstrap");
    _b_log.raw(Splash::get_startup_splash() + "\n");
    _b_log.raw("Toast Loaded on OS: [" + Environment::OS::to_string() + "] with Process ID [" + std::to_string(get_pid()) + "]");
    _b_log.raw("Hardware Provider: " + std::string(info->name));
    _b_cfg.reload();
	Bootstrap::Web::prepare();
    
    // State Tick Timing (50Hz)
    int tick_frequency = (int)(1000.0 / _b_cfg.get_double("timings.states.frequency", 50.0));
    States::Internal::set_tick_timing(tick_frequency);
    
	if (load) {
		_b_log << "Initializing Loader";
		Bootstrap::Loader::initialize();
	}

    Bootstrap::States::init();
	Bootstrap::Web::start();

    provider_init();
    long end_time = current_time_millis();
    _b_log << "Total Bootstrap Startup Time: " + to_string(end_time - start_time) + "ms";

    if (loop) provider_loop();
	bootstrap_shutdown();
	CRASH_HANDLE_END
}

Toast::Config *Bootstrap::get_config() {
	return &_b_cfg;
}

Toast::Logger *Bootstrap::get_logger() {
	return &_b_log;
}

#include "toast/net/socket.hpp"
#include "toast/bootstrap.hpp"
#include "toast/bootstrap/loader.hpp"
#include "toast/bootstrap/state.hpp"
#include "toast/memory.hpp"
#include "toast/filesystem.hpp"
#include "toast/logger.hpp"
#include "toast/splash.hpp"
#include "toast/environment.hpp"
#include "toast/state.hpp"
#include "toast/util.hpp"
#include "toast/crash.hpp"
#include "toast/config.hpp"

#include "provider/provider.hpp"

using namespace Toast;

Logger _b_log("Toast-Bootstrap");
Config _b_cfg("Toast-Bootstrap");

void init_toast_bootstrap(int argc, char *argv[]) {
    ProviderInfo *info = provider_info();
    Crash::initialize();
    
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
    
    // State Tick Timing (50Hz)
    int tick_frequency = (int)(1000.0 / _b_cfg.get_double("timings.states.frequency", 50.0));
    States::Internal::set_tick_timing(tick_frequency);
    
    _b_log << "Initializing Loader";
    Bootstrap::Loader::initialize();

    Bootstrap::States::init();
    provider_init();
    
    CRASH_HANDLE_END
}
#include "toast/net/socket.hpp"
#include "toast/bootstrap.hpp"
#include "toast/bootstrap/loader.hpp"
#include "toast/memory.hpp"
#include "toast/filesystem.hpp"
#include "toast/logger.hpp"
#include "toast/bootstrap/splash.hpp"
#include "toast/environment.hpp"
#include "toast/state.hpp"
#include "toast/util.hpp"

using namespace Toast;

Logger _b_log("Toast-Bootstrap");

void init_toast_bootstrap(int argc, char *argv[]) {
    Net::Socket::socket_init();
    Filesystem::initialize();
    Memory::initialize_bootstrap();
    
    Log::initialize("Bootstrap");
    _b_log.raw(Splash::get_startup_splash() + "\n");
    _b_log.raw("Toast Loaded on OS: [" + Environment::OS::to_string() + "] with Process ID [" + std::to_string(get_pid()) + "]");
    
    // Default Tick Timing (50Hz)
    States::Internal::set_tick_timing(20);
    
    _b_log << "Initializing Loader";
    Bootstrap::Loader::initialize();
}
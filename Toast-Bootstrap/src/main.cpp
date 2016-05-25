#include "toast/bootstrap.hpp"
#include "toast/bootstrap/loader.hpp"
#include "toast/memory.hpp"
#include "toast/filesystem.hpp"
#include "toast/logger.hpp"
#include "toast/bootstrap/splash.hpp"
#include "toast/environment.hpp"

using namespace Toast;

Logger _b_log("Toast-Bootstrap");

void init_toast_bootstrap(int argc, char *argv[]) {
    Filesystem::initialize();
    Memory::initialize_bootstrap();
    
    Log::initialize("Bootstrap");
    _b_log.raw(Splash::get_startup_splash() + "\n");
    _b_log.raw("Toast Loaded on OS: [" + Environment::OS::to_string() + "]");
    
    _b_log << "Initializing Loader";
    Bootstrap::Loader::initialize();
}
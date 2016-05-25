#include "toast/bootstrap.hpp"
#include "toast/bootstrap/loader.hpp"
#include "toast/memory.hpp"
#include "toast/filesystem.hpp"
#include "toast/logger.hpp"

using namespace Toast;

Logger __bootstrap_logger("Toast-Bootstrap");

void init_toast_bootstrap(int argc, char *argv[]) {
    Filesystem::initialize();
    Memory::initialize_bootstrap();
    
    Log::initialize("Bootstrap");
    
    Bootstrap::Loader::initialize();
}
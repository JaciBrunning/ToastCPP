#include "toast/bootstrap.hpp"
#include "toast/bootstrap/loader.hpp"
#include "toast/bootstrap/memory.hpp"
#include "toast/filesystem.hpp"
#include "toast/logger.hpp"

using namespace Toast;

Logger __bootstrap_logger("Toast-Bootstrap");

void init_toast_bootstrap(int argc, char *argv[]) {
    Filesystem::initialize();
    Log::initialize("Bootstrap");
    
    __bootstrap_logger.info("Hello World");
    
    Bootstrap::Memory::initialize();
    Bootstrap::Loader::initialize();
}
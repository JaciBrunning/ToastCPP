#include "toast/bootstrap.hpp"
#include "toast/bootstrap/loader.hpp"
#include "toast/bootstrap/memory.hpp"
#include "toast/filesystem.hpp"

void init_toast_bootstrap(int argc, char *argv[]) {
    Toast::Bootstrap::Memory::initialize();
    Toast::Filesystem::initialize();
    Toast::Bootstrap::Loader::initialize();
}
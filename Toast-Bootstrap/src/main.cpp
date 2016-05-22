#include "toast/bootstrap.hpp"
#include "toast/bootstrap/loader.hpp"
#include "toast/filesystem.hpp"

void init_toast_bootstrap(int argc, char *argv[]) {
    Toast::Filesystem::initialize();
    Toast::Bootstrap::Loader::initialize();
}
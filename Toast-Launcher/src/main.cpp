#include "toast/environment.hpp"
#include "toast/internal/loader.hpp"
#include "toast/util.hpp"

#include <string>
#include <iostream>
#include <string.h>

using namespace Toast;
using namespace Toast::Internal;
using namespace std;

int main(int argc, char *argv[]) {
    Loader::initialize();
    
    bool module(false);
    if (argc > 1) {
        if (strcmp(argv[1], "__TOAST_MODULE_LD_FILE") == 0) {
            module = true;
        }
    }
    if (!module) {
        DYNAMIC dyn = Loader::load_dynamic_library(Loader::library_name("toast_bootstrap"));
        SYMBOL sym = Loader::get_symbol(dyn, "init_toast_bootstrap");
        reinterpret_cast<void (*)(int, char*[])>(sym)(argc, argv);
    } else {
        cout << "Module! " << argv[2] << endl;
    }
    return 0;
}
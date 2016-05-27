#include "toast/module.hpp"
#include "toast/memory.hpp"

#include "toast/logger.hpp"

using namespace std;

vector<Toast::ModuleData> Toast::get_all_modules() {
    vector<Toast::ModuleData> v;
    int i;
    for (i = 0x10; i < 0x10 + 256; i++) {
        int module_stat = (int) Toast::Memory::Shared::get()[i];
        if (module_stat != 0x00) {
            // Module found at the least
            char buf[TOAST_PRIVATE_MEMPOOL_SIZE];
            Toast::Memory::copy_private_pool(i - 0x10, &buf[0]);
            char name[50];
            memcpy((char *)&name, &buf[0x10], 50);
            string name_str(name);
            char file[256];
            memcpy((char *)&file, &buf[0x100], 256);
            string file_str(file);
            
            Toast::ModuleData d = {name_str, file_str, i - 0x10, module_stat};
            v.push_back(d);
        }
    }
    return v;
}
#include "toast/bootstrap/loader.hpp"
#include "toast/filesystem.hpp"
#include "toast/util.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace Toast;
using namespace Toast::Bootstrap;
using namespace std;

static vector<Loader::ModuleAdapter *> __modules;

void Loader::initialize() {
    Loader::search_modules();
    Loader::create_subprocesses();
}

void Loader::search_modules() {
    vector<string> module_files = Filesystem::ls(Filesystem::path("modules"));
    for (auto module : module_files) {
        if (ends_with(module, OS_LIB)) {
            Loader::ModuleAdapter *adapter = new Loader::ModuleAdapter();
            adapter->file = module;
            adapter->given_unq = Loader::super_horrible_hash_func(adapter->file);
            __modules.push_back(adapter);
        }
    }
}

void Loader::create_subprocesses() {
    for (auto module : __modules) {
        Loader::create_module_process(module);
    }
    sleep_ms(1000);
}

static void create_process_thread(Loader::ModuleAdapter *adapter) {
    create_specific_mempool(adapter);
    string mempool_handle = adapter->given_unq;
    string file = adapter->file;
    
    cout << "Part 1 " << endl;
    char out[] = {0x7A};
    // memcpy(adapter->specific_mempool, out, 1);
    adapter->specific_mempool[0] = 0x7A;    // Verification Byte
    cout << "Part 2" << endl;
    
    #ifdef OS_WIN
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );
        
        if (CreateProcess(NULL, 
            (char *)("toast_launcher __TOAST_MODULE_LD_FILE " + file + " " + mempool_handle).c_str(), 
            NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    #else
        char *args[] {
            (char *)("./toast_launcher"),
            (char *)("__TOAST_MODULE_LD_FILE"),
            (char *)file.c_str(), 
            (char *)mempool_handle.c_str(), 
            NULL
        };
        execv("./toast_launcher", args);
    #endif
}

void Loader::create_module_process(Loader::ModuleAdapter *adapter) {
    thread t(create_process_thread, adapter);
    t.detach();
}

string Loader::super_horrible_hash_func(string original) {
    unsigned int h = 31;
    const char *s = original.c_str();
    while (*s) {
        h = (h * 54059) ^ (s[0] * 76963);
        s++;
    }
    stringstream stream;
    stream << hex << h;
    string result(stream.str());
    return result;
}

void Loader::create_specific_mempool(Loader::ModuleAdapter *adapter) {
    adapter->specific_mempool_handle = Internal::SHM::create_shm_file(adapter->given_unq, TOAST_SPECIFIC_MEMPOOL_SIZE);
    adapter->specific_mempool = Internal::SHM::map_shm_file(adapter->specific_mempool_handle, TOAST_SPECIFIC_MEMPOOL_SIZE);
}

void Loader::free_specific_mempool(Loader::ModuleAdapter *adapter) {
    Internal::SHM::unmap_shm_file(adapter->specific_mempool, TOAST_SPECIFIC_MEMPOOL_SIZE);
    Internal::SHM::close_shm_file(adapter->given_unq, adapter->specific_mempool_handle);
}
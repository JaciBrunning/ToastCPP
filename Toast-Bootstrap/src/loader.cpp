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
    vector<string> module_files = Filesystem::ls_local(Filesystem::path("modules"));
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

static void create_process(Loader::ModuleAdapter *adapter) {
    string mempool_handle = adapter->given_unq;
    string file = adapter->file;
    
    adapter->private_mempool[0] = PMP_VERIFY;    // Verification Byte
    
    #ifdef OS_WIN
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        
        if (CreateProcess(NULL, 
            (char *)("toast_launcher __TOAST_MODULE_LD_FILE " + file + " " + mempool_handle).c_str(), 
            NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    #else
        system(("./toast_launcher __TOAST_MODULE_LD_FILE " + file + " " + mempool_handle).c_str());
    #endif
}

static void create_process_thread(Loader::ModuleAdapter *adapter) {
    create_private_mempool(adapter);
    do {
        create_process(adapter);
        char name[50];
        memcpy(name, &adapter->private_mempool[10], 50);
        cout << "Name: " << name << endl;
        memcpy(name, &adapter->private_mempool[60], 50);
        cout << "Unique: " << name << endl;
    } while (adapter->private_mempool[1] == 0x01);
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

void Loader::create_private_mempool(Loader::ModuleAdapter *adapter) {
    adapter->private_mempool_handle = Internal::SHM::create_shm_file(adapter->given_unq, TOAST_PRIVATE_MEMPOOL_SIZE);
    adapter->private_mempool = Internal::SHM::map_shm_file(adapter->private_mempool_handle, TOAST_PRIVATE_MEMPOOL_SIZE);
}

void Loader::free_private_mempool(Loader::ModuleAdapter *adapter) {
    Internal::SHM::unmap_shm_file(adapter->private_mempool, TOAST_PRIVATE_MEMPOOL_SIZE);
    Internal::SHM::close_shm_file(adapter->given_unq, adapter->private_mempool_handle);
}
#include "toast/bootstrap/loader.hpp"
#include "toast/filesystem.hpp"
#include "toast/util.hpp"

#include <thread>
#include <iostream>
#include <stdlib.h>

using namespace Toast;
using namespace Toast::Bootstrap;
using namespace std;

static vector<string> __module_files;

void Loader::initialize() {
    Loader::search_modules();
    Loader::create_subprocesses();
}

void Loader::search_modules() {
    vector<string> module_files = Filesystem::ls(Filesystem::path("modules"));
    for (auto module : module_files) {
        if (ends_with(module, OS_LIB)) {
            __module_files.push_back(module);
        }
    }
}

static void create_process_thread(string file) {
    cout << "Launching Process for Module File: " << file << endl;
    #ifdef OS_WIN
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );
        
        if (CreateProcess(NULL, (char *)("toast_launcher __TOAST_MODULE_LD_FILE " + file).c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    #else
        char *args[] {
            (char *)("./toast_launcher"),
            (char *)("__TOAST_MODULE_LD_FILE"),
            (char *)file.c_str(), NULL
        };
        execv("./toast_launcher", args);
    #endif
}

void Loader::create_subprocesses() {
    for (auto module : __module_files) {
        thread t(create_process_thread, module);
        t.detach();
    }
    cout << "I'm still running!" << endl;
    sleep_ms(10000);
}
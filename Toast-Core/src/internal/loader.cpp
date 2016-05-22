#include "toast/internal/loader.hpp"

using namespace Toast::Internal;
using namespace std;

#ifdef OS_UNIX
DYNAMIC Loader::Platform::unix_dlopen(char *file) {
    return dlopen(file, RTLD_LAZY);
}
#endif

#ifdef OS_WIN
string Loader::Platform::win_dlerror() {
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::string();

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    string message(messageBuffer, size);

    LocalFree(messageBuffer);

    return message;
}
#endif

void Loader::initialize() {
    #ifdef OS_WIN
        // Windows doesn't support -rpath
        AddDllDirectory(L".");
        AddDllDirectory(L"./toast/modules");
        AddDllDirectory(L"./toast/libs");
    #endif
}

DYNAMIC Loader::load_dynamic_library(string file) {
    return DYN_LD((char *)file.c_str());
}

SYMBOL Loader::get_symbol(DYNAMIC library, string sym) {
    return DYN_SYM(library, (char *)sym.c_str());
}

string Loader::get_dynamic_error() {
    return string(DYN_ERR());
}

string Loader::library_name(string library) {
    string name = OS_PREFIX;
    name += library;
    name += OS_SUFFIX;
    return name;
}
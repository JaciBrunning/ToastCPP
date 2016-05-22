#include "toast/environment.hpp"
#include "toast/filesystem.hpp"

#ifdef OS_WIN
    #include "compat/win32/dirent.h"
    #include <direct.h>
    #include <windows.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <dirent.h>
#endif

using namespace Toast;
using namespace std;

string Filesystem::join(string path1, string path2) {
    string total_path = path1;
    if (!path1.empty() && path1.back() != '/')
        total_path += "/";
    total_path += path2;
    return total_path;
}

string Filesystem::path(string path) {
    return Filesystem::join(Environment::toast_home(), path);
}

string Filesystem::path_mkdir(string path) {
    string p = Filesystem::path(path);
    Filesystem::mkdir(p);
    return p;
}

string Filesystem::absolute(string path) {
    char current_directory[256];
    #ifdef OS_WIN
        GetCurrentDirectory(256, current_directory);
    #else
        getcwd(current_directory, 256);
    #endif
    return Filesystem::join(string(current_directory), path);
}

void Filesystem::rmdir(string path) {
    DIR *dir;
    struct dirent *ent;
    char filepath[256];
    
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (0==strcmp(ent->d_name, ".") || 0==strcmp(ent->d_name, "..")) { continue; }
            
            sprintf(filepath, "%s/%s", path.c_str(), ent->d_name);
            Filesystem::rmdir(string(filepath));
            remove(filepath);
        }
        closedir(dir);
    };
}

void Filesystem::mkdir(string path) {
    #ifdef OS_WIN
        _mkdir((char *)path.c_str());
    #else
        ::mkdir((char *)path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    #endif
}

vector<string> Filesystem::ls(string path) {
    DIR *dir;
    struct dirent *ent;
    vector<string> v;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                v.push_back(Filesystem::join(path, ent->d_name));
        }
        closedir (dir);
    };
    return v;
}

vector<string> Filesystem::ls_local(string path) {
    DIR *dir;
    struct dirent *ent;
    vector<string> v;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                v.push_back(ent->d_name);
        }
        closedir (dir);
    };
    return v;
}

bool initialized(false);

void Filesystem::initialize() {
    if (!initialized) {
        Filesystem::mkdir(Filesystem::path(""));
        Filesystem::mkdir(Filesystem::path("modules"));
        Filesystem::mkdir(Filesystem::path("libs"));
        Filesystem::mkdir(Filesystem::path("internal"));
        
        Filesystem::rmdir(Filesystem::path("internal/volatile"));
        Filesystem::mkdir(Filesystem::path("internal/volatile"));
        initialized = true;
    }
}
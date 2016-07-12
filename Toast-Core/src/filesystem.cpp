#include "toast/environment.hpp"
#include "toast/filesystem.hpp"

#ifdef OS_WIN
    #include "compat/win32/dirent.h"
    #include <direct.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <dirent.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>

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

bool Filesystem::exists(string path) {
	struct stat info;
	if (stat(path.c_str(), &info) != 0) {
		return false;
	} else {
		return true;
	}
}

bool Filesystem::is_directory(string path) {
	struct stat info;
	if (stat(path.c_str(), &info) == 0 && info.st_mode & S_IFDIR) {
		return true;
	}
	return false;
}

string Filesystem::extension(string path) {
	return path.substr(path.find_last_of(".") + 1);
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
        Filesystem::mkdir(Filesystem::path("config"));
        
        Filesystem::rmdir(Filesystem::path("internal/volatile"));
        Filesystem::mkdir(Filesystem::path("internal/volatile"));
        initialized = true;
    }
}
#include "toast/environment.hpp"
#include "toast/filesystem.hpp"
#include "toast/util.hpp"

#ifdef OS_WIN
    #include "compat/win32/dirent.h"
    #include <direct.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <dirent.h>
	#include <pwd.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>

using namespace Toast;
using namespace std;

static std::string fix_win_sep(std::string &str) {
	std::replace(str.begin(), str.end(), '\\', '/');
	return str;
}

string Filesystem::toast_dir() {
	return Environment::toast_home();
}

string Filesystem::user_dir() {
	return Environment::user_home();
}

string Filesystem::join(string path1, string path2) {
    string total_path = path1;
    if (!path1.empty() && path1.back() != '/')
        total_path += "/";
    total_path += path2;
    return fix_win_sep(total_path);
}

string Filesystem::path(string path) {
    return Filesystem::join(Filesystem::toast_dir(), path);
}

string Filesystem::path_mkdir(string path) {
    string p = Filesystem::path(path);
    Filesystem::mkdir(p);
    return p;
}

string Filesystem::absolute(string path) {
	if (path.length() > 0) {
		if (path[0] == '/') return path;
		else if (path[0] == '~' && path[1] == '/') {
			return Filesystem::join(Filesystem::user_dir(), path.substr(2));
		} else if (path.length() > 2 && path[1] == ':') {	// Windows drive
			return fix_win_sep(path);
		}
	}

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

void Filesystem::rm(string path) {
	remove(path.c_str());
}

string Filesystem::extension(string path) {
	return path.substr(path.find_last_of(".") + 1);
}

vector<string> Filesystem::split_path(string path) {
	std::vector<std::string> result;

	char const* pch = path.c_str();
	char const* start = pch;
	for (; *pch; ++pch) {
		if (*pch == '/') {
			if (start != pch) {
				std::string str(start, pch);
				result.push_back(str);
			} else {
				result.push_back("");
			}
			start = pch + 1;
		}
	}
	result.push_back(start);

	return result;
}

string Filesystem::name(string path) {
	return Filesystem::split_path(path).back();
}

string Filesystem::basename(string path) {
	string name = Filesystem::name(path);
	return name.substr(0, name.find_last_of("."));
}

string Filesystem::parent(string path) {
	vector<string> spl = Filesystem::split_path(path);
	spl.pop_back();
	string total = spl[0];
	for (int i = 1; i < spl.size(); i++) total += "/" + spl[i];
	return total;
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
		Filesystem::mkdir(Filesystem::path("drivers"));
        Filesystem::mkdir(Filesystem::path("internal"));
        Filesystem::mkdir(Filesystem::path("config"));
        
        Filesystem::rmdir(Filesystem::path("internal/volatile"));
        Filesystem::mkdir(Filesystem::path("internal/volatile"));
        initialized = true;
    }
}
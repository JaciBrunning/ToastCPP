#include "toast/util.hpp"

bool ends_with(std::string const & value, std::string const & ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool starts_with(std::string const & value, std::string const & starting) {
	return !value.compare(0, starting.size(), starting);
}

void sleep_ms(long milliseconds) {
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}

long long current_time_millis() {
#ifdef WIN32
	FILETIME time;
	GetSystemTimeAsFileTime(&time);
	return (long long)(((LONGLONG)time.dwLowDateTime + ((LONGLONG)(time.dwHighDateTime) << 32LL) - 116444736000000000LL) / 10000LL);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
	return (long long)((unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000);
#endif
}

long current_cpu_time() {
#ifdef WIN32
    FILETIME a,b,c,d;
    if (GetProcessTimes(GetCurrentProcess(),&a,&b,&c,&d) != 0){
        return
            (long)((d.dwLowDateTime |
            ((unsigned long long)d.dwHighDateTime << 32)) / 10000);
    } else {
        return 0;
    }
#else
    return (long)((double)clock() / CLOCKS_PER_SEC * 1000);
#endif
}

int get_pid() {
    #ifdef OS_WIN
        return GetCurrentProcessId();
    #else
        return getpid();
    #endif
}

bool is_process_alive(int pid) {
    #ifdef OS_WIN
        DWORD exCode;
        HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (GetExitCodeProcess(processHandle, &exCode)) {
            CloseHandle(processHandle);
            if (exCode == STILL_ACTIVE) return true;
            return false;
        } else {
            CloseHandle(processHandle);
            return false;
        }
    #else
        int ret = kill(pid, 0);
        if (ret == -1 && errno == ESRCH) {
            return false;
        }
        return true;
    #endif
}

std::vector<std::string> &split_internal(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split_internal(s, delim, elems);
    return elems;
}
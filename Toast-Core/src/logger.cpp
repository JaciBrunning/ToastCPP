#include "toast/logger.hpp"
#include "toast/filesystem.hpp"
#include "toast/memory.hpp"
#include "toast/state.hpp"

#include <string.h>

using namespace Toast;
using namespace std;

static string _process_name;
static Concurrent::Mutex local_mutex;
static bool _debug;

static string _log_file;

static ofstream file_out;

Log::Level _INFO      = {"INFO", false, false};
Log::Level _WARN      = {"WARN", false, false};
Log::Level _ERR       = {"ERROR", true, false};
Log::Level _SEVERE    = {"SEVERE", true, false};
Log::Level _DEBUGL     = {"DEBUG", false, true};

Log::Level Log::INFO() {
    return _INFO;
}

Log::Level Log::WARN() {
    return _WARN;
}

Log::Level Log::ERR() {
    return _ERR;
}

Log::Level Log::SEVERE() {
    return _SEVERE;
}

Log::Level Log::DEBUG() {
    return _DEBUGL;
}

static char buf[80];

const string timeFormat() {
    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);
    
    strftime(buf, sizeof(buf), "%d/%m/%Y-%H:%M:%S", &tstruct);
    
    return buf;
}

void backup_last_log() {
    string destination = Filesystem::path("log/last/" + _process_name + ".tlog");
    Log::copyTo(destination);
}

void Log::initialize(string process_name) {
    _process_name = process_name;
    Filesystem::path_mkdir("log");
    Filesystem::path_mkdir("log/current");
    Filesystem::path_mkdir("log/last");
    _log_file = Filesystem::path("log/current/" + process_name + ".tlog");
    backup_last_log();
    
    file_out.open(_log_file);
    if (file_out.fail()) {
        Log::log("Logger", "Could not initialize Logger File Output! " + string(strerror(errno)), Log::ERR());
    }
}

void Log::set_debug(bool is_debug) {
    _debug = is_debug;
}

void Log::flush() {
    file_out.flush();
}

void Log::close() {
    file_out.flush();
    file_out.close();
}

string Log::file() {
    return _log_file;
}

void Log::copyTo(string path) {
    ifstream src(Log::file(), ios::binary);
    ofstream dst(path, ios::binary);
    
    dst << src.rdbuf();
}

void Log::log(string name, string msg, Log::Level level) {
    string formatted = "[" + timeFormat() + "] [" + name + "] [" + level.name + "] " + msg;
    Log::log_raw(name, level.name, msg, formatted, level.is_error, level.is_debug);
}

void Log::log_raw(string sender, string level, string raw, string msg, bool error, bool debug) {
	long sysmillis = current_time_millis();
    if (!debug || _debug || Memory::shared()->get_debug()) {
		Concurrent::IPCMutex *logger_mutex = Memory::shared_mutex()->logger;
		if (logger_mutex != nullptr) logger_mutex->lock(0);
        if (error) {
            cerr << msg << endl;
        } else {
            cout << msg << endl;
        }
		if (logger_mutex != nullptr) logger_mutex->unlock(0);
    }

	// TODO Async this
    if (file_out.is_open()) {
		local_mutex.lock();
		stringstream ss(raw);
		string tmp;
		while (getline(ss, tmp, '\n')) {
			file_out <<
				sysmillis << ";" <<
				(int)error << ";" << (int)debug << ";" <<
				(int)States::current_robotstate() << ";" <<
				sender << ";" <<
				level << " " <<
				tmp << endl;
		}
		local_mutex.unlock();
    }
}

void Log::info(string name, string msg) {
    Log::log(name, msg, _INFO);
}

void Log::debug(string name, string msg) {
    Log::log(name, msg, _DEBUGL);
}

Logger::Logger(string n) {
    name = n;
}

void Logger::log(string msg, Log::Level level) {
    Log::log(name, msg, level);
}

void Logger::info(string msg) {
    log(msg, Log::INFO());
}

void Logger::debug(string msg) {
    log(msg, Log::DEBUG());
}

void Logger::warn(string msg) {
    log(msg, Log::WARN());
}

void Logger::error(string msg) {
    log(msg, Log::ERR());
}

void Logger::severe(string msg) {
    log(msg, Log::SEVERE());
}

void Logger::raw(string msg, bool error, bool debug) {
    Log::log_raw(name, "RAW", msg, msg, error, debug);
}

void Logger::raw(string msg) {
    Logger::raw(msg, false, false);
}

void Logger::operator<<(string msg) {
    Logger::info(msg);
}

void Logger::set_name(string n) {
    name = n;
}

string Logger::get_name() {
    return name;
}
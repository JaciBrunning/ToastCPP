#include "toast/logger.hpp"
#include "toast/filesystem.hpp"
#include "toast/memory.hpp"

#include <string.h>

using namespace Toast;
using namespace std;

static string _process_name;
static bool _debug;

// 512 KB ought to be enough
static char buffer[1024 * 512];

ofstream file_out;

Log::Level _INFO      = {"INFO", false, false};
Log::Level _WARN      = {"WARN", false, false};
Log::Level _ERR       = {"ERROR", true, false};
Log::Level _SEVERE    = {"SEVERE", true, false};
Log::Level _DEBUG     = {"DEBUG", false, true};

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
    return _DEBUG;
}

static char buf[80];

const string timeFormat() {
    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);
    
    strftime(buf, sizeof(buf), "%d/%m/%Y-%H:%M:%S", &tstruct);
    
    return buf;
}

void Log::initialize(string process_name) {
    _process_name = process_name;
    Filesystem::path_mkdir("log");
    Filesystem::path_mkdir("log/current");
    file_out.open(Filesystem::path("log/current/" + process_name + ".tlog"));
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

void Log::log(string name, string msg, Log::Level level) {
    string formatted = "[" + timeFormat() + "] [" + name + "] [" + level.name + "] " + msg;
    Log::log_raw(formatted, level.is_error, level.is_debug);
}

void Log::log_raw(string msg, bool error, bool debug) {
    if (!debug || _debug || Memory::Shared::get_debug()) {
        if (error) {
            cerr << msg << endl;
        } else {
            cerr << msg << endl;
        }
    }
    
    int total_length = 30 + msg.length();
    buffer[0] = 0x00; // Code
    
    int now = (int)current_time_millis();
    
    Transport::intToBytes(now, buffer, 0x01);
    //memcpy(&buffer[0x05], );           TODO MatchTime here
    //                                   TODO RobotState here
    char flags = 0x00;
    if (error) flags = flags | 1;
    if (debug) flags = flags | (1 << 1);
    
    Transport::intToBytes(msg.length(), buffer, 0x1A);
    memcpy(&buffer[0x1E], msg.c_str(), msg.length());
    
    if (file_out.is_open()) {
        file_out.write(buffer, total_length);
    }
}

void Log::info(string name, string msg) {
    Log::log(name, msg, _INFO);
}

void Log::debug(string name, string msg) {
    Log::log(name, msg, _DEBUG);
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
    Log::log_raw(msg, error, debug);
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
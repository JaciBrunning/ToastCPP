#include "toast/bootstrap/utils/log.hpp"

#include "toast/filesystem.hpp"
#include "toast/state.hpp"

#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace BootstrapUtil;
using namespace std;
using namespace Toast;

static vector<string> files;
static string out;

struct tLogMsg {
	int id;
	std::string process;
	long time_ms;
	bool error, debug;
	State state;
	std::string sender, level, msg;
};

static bool compareTLogMsgTime(const tLogMsg &a, const tLogMsg &b) {
	return a.time_ms < b.time_ms;
}

static vector<tLogMsg> get_msgs() {
	vector<tLogMsg> msgs;
	for (string infile : files) {
		string process = Filesystem::basename(infile);
		ifstream file_in(infile);

		string line;

		while (std::getline(file_in, line, ';')) {
			tLogMsg msg = {
				0, process,
				stol(line),
				false, false, States::DISABLED(), "", "", ""
			};

			stringstream stream(line);
			std::getline(file_in, line, ';');
			msg.error = stoi(line) == 0 ? false : true;
			std::getline(file_in, line, ';');
			msg.debug = stoi(line) == 0 ? false : true;
			std::getline(file_in, line, ';');
			msg.state = States::from_robotstate((RobotState)stoi(line));
			std::getline(file_in, line, ';');
			msg.sender = line;
			std::getline(file_in, line, ' ');
			msg.level = line;
			std::getline(file_in, line, '\n');
			msg.msg = line;

			msgs.push_back(msg);
		}
	}
	std::sort(msgs.begin(), msgs.end(), compareTLogMsgTime);
	for (int i = 0; i < msgs.size(); i++) {
		msgs[i].id = i;
	}
	return msgs;
}

// Don't use get_msgs, it's faster to just do a direct pipe.
static void log2csv() {
	for (string infile : files) {
		string outfile_name = Filesystem::basename(infile) + ".csv";
		string outfile = Filesystem::join(Filesystem::parent(infile), outfile_name);
		if (!out.empty()) outfile = Filesystem::join(out, outfile_name);

		ifstream file_in(infile);
		ofstream file_out(outfile);

		file_out << "Id,Time,Error,Debug,State,Sender,Level,Msg" << endl;
		int sequenceid = 0;
		while (!file_in.eof()) {
			string tmp;
			if (!std::getline(file_in, tmp, ';')) break;
			file_out << sequenceid << ",";
			file_out << tmp << ",";

			for (int i = 0; i < 4; i++) {
				std::getline(file_in, tmp, ';');
				if (i == 2) {
					// States
					int l = stoi(tmp);
					tmp = States::from_robotstate((RobotState)l).to_string();
				}
				file_out << tmp << ",";
			}
			std::getline(file_in, tmp, ' ');
			file_out << tmp << ",";

			std::getline(file_in, tmp, '\n');

			// Replace double quotes in log with single quotes
			std::replace(tmp.begin(), tmp.end(), '"', '\'');
			file_out << "\"" << tmp << "\"" << endl;
			sequenceid++;
		}

		file_in.close();
		file_out.close();
	}
}

static bool common_argparse(int argc, char *argv[], int argi, bool output_directory) {
	for (int i = argi + 1; i < argc; i++) {
		string a(argv[i]);

		if (a == "-o") {
			if (i == argc - 1) {
				cerr << "[ERR] You must specify an output directory for the -o switch!" << endl;
				return false;
			} else {
				i++;
				string b(argv[i]);
				string _raw = Filesystem::absolute(b);

				bool valid = Filesystem::exists(_raw) && (output_directory ? Filesystem::is_directory(_raw) : true);
				if (valid) {
					out = _raw;
				} else {
					cerr << "[ERR] Output folder does not exist or is file: " << _raw << endl;
					return false;
				}
			}
		} else {
			string _raw = Filesystem::absolute(a);
			if (Filesystem::exists(_raw)) {
				if (Filesystem::is_directory(_raw)) {
					for (string _r : Filesystem::ls(_raw)) {
						if (!Filesystem::is_directory(_r) && Filesystem::extension(_r) == "tlog")
							files.push_back(_r);
					}
				} else {
					files.push_back(_raw);
				}
			} else {
				cerr << "[ERR] File does not exist: " << _raw << endl;
				return false;
			}
		}
	}

	if (files.empty()) {
		cerr << "[ERR] No files specified!" << endl;
		return false;
	}

	return true;
}

void Log::log2something(int argc, char *argv[], int argi) {
	common_argparse(argc, argv, argi, true);

	char *a = argv[argi];
	if (strcmp(a, "log2csv") == 0) {
		log2csv();
	} else {
		cerr << "[ERR] Log Conversion mode " << a << " is not recognised!" << endl;
	}
}

void Log::combine_logs(int argc, char *argv[], int argi) {
	common_argparse(argc, argv, argi, false);

	if (out.empty()) out = Filesystem::absolute("combined.csv");
	else if (Filesystem::is_directory(out)) out = Filesystem::join(out, "combined.csv");

	ofstream file_out(out);
	file_out << "Id,Time,Error,Debug,State,Process,Sender,Level,Msg" << endl;
	int id = 0;
	for (tLogMsg msg : get_msgs()) {
		file_out << id++ <<
			"," << msg.time_ms <<
			"," << (msg.error ? 1 : 0) <<
			"," << (msg.debug ? 1 : 0) <<
			"," << (msg.state.to_string()) <<
			"," << msg.process <<
			"," << msg.sender <<
			"," << msg.level <<
			"," << msg.msg << endl;
	}
}
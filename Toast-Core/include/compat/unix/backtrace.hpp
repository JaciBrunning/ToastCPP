#pragma once

#include "toast/environment.hpp"

#ifndef OS_WIN
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>

static std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "";
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
		return "";
    }
    pclose(pipe);
    return result;
}

std::vector<std::string> backtrace_get(int skip) {
    void *callstack[128];
	const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
	char buf[1024];
	int nFrames = backtrace(callstack, nMaxFrames);
	char **symbols = backtrace_symbols(callstack, nFrames);

    std::vector<std::string> vs;
	for (int i = skip; i < nFrames; i++) {
		// printf("%s\n", symbols[i]);
		Dl_info info;
		if (dladdr(callstack[i], &info)) {
			char *demangled = NULL;
			int status = -1;
			if (info.dli_sname && info.dli_sname[0] == '_')
				demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
            //sprintf(buf, "%-3d %*p %s %s 0x%02X + 0x%02X",
            //    nFrames - i - 1, int(2 + sizeof(void*) * 2), callstack[i],
			//	info.dli_fname,
            //    status == 0 ? demangled :
            //    info.dli_sname == 0 ? symbols[i] : info.dli_sname,
			//	(char *)info.dli_fbase,
            //    (char *)callstack[i] - (char *)info.dli_saddr);
			void *relative_ptr = (void *)(((char *)callstack[i]) - (char *)info.dli_fbase);
			sprintf(buf, "%-3d %*p %s(%s + %p)",
				nFrames - i - 1,
				int(2 + sizeof(void*) * 2), callstack[i],
				info.dli_fname,
				status == 0 ? demangled : (info.dli_sname == 0 ? "<static member>" : info.dli_sname),
				relative_ptr);
			
            free(demangled);
            vs.push_back(std::string(buf));
			
//			char syscom[256];
//			sprintf(syscom,"addr2line -e %s %p 2> /dev/null", info.dli_fname, relative_ptr);	// Linux addr2line
//			std::string exec_return = exec(syscom);
//			if (!exec_return.empty()) {
//				exec_return.erase(exec_return.length() - 1);	// Remove \n
//				sprintf(buf, "\t\t%s", exec_return.c_str());
//				vs.push_back(std::string(buf));
//			} else {
//				sprintf(syscom, "atos -o %s %p 2> /dev/null", info.dli_fname, relative_ptr);	// Mac atos
//				exec_return = exec(syscom);
//				if (!exec_return.empty()) {
//					exec_return.erase(exec_return.length() - 1);	// Remove \n
//					sprintf(buf, "\t\t%s", exec_return.c_str());
//					vs.push_back(std::string(buf));
//				}
//			}
        } else {
            sprintf(buf, "%-3d %*p %s [No DL_Info]",
                    nFrames - i - 1, int(2 + sizeof(void*) * 2), callstack[i], symbols[i]);
            vs.push_back(std::string(buf));
        }
    }
    free(symbols);
    if (nFrames == nMaxFrames) {
        vs.push_back("[truncated]");
    }
    return vs;
}

#endif
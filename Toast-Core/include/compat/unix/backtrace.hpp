#pragma once

#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>

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
		if (dladdr(callstack[i], &info) && info.dli_sname) {
			char *demangled = NULL;
			int status = -1;
			if (info.dli_sname[0] == '_')
				demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
            sprintf(buf, "%-3d %*p %s + %zd",
                nFrames - i - 1, int(2 + sizeof(void*) * 2), callstack[i],
                status == 0 ? demangled :
                info.dli_sname == 0 ? symbols[i] : info.dli_sname,
                (char *)callstack[i] - (char *)info.dli_saddr);
            free(demangled);
            vs.push_back(std::string(buf));
        } else {
            sprintf(buf, "%-3d: %*p %s",
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
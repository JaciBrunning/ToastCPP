#pragma once

#include "Windows.h"
#include "DbgHelp.h"
#include <WinBase.h>
#pragma comment(lib, "dbghelp.lib") 

#include <vector>
#include <string>

std::vector<std::string> backtrace_get(int skip) {
     unsigned int   i;
     void         * stack[ 100 ];
     unsigned short frames;
     SYMBOL_INFO  * symbol;
     HANDLE         process;
	 IMAGEHLP_LINE *line = (IMAGEHLP_LINE *)malloc(sizeof(IMAGEHLP_LINE));
	 line->SizeOfStruct = sizeof(IMAGEHLP_LINE);
	 IMAGEHLP_LINE64 *line64 = (IMAGEHLP_LINE64 *)malloc(sizeof(IMAGEHLP_LINE64));
	 line64->SizeOfStruct = sizeof(IMAGEHLP_LINE64);

	 IMAGEHLP_MODULE *mod = (IMAGEHLP_MODULE *)malloc(sizeof(IMAGEHLP_MODULE));
	 mod->SizeOfStruct = sizeof(IMAGEHLP_MODULE);
	 IMAGEHLP_MODULE64 *mod64 = (IMAGEHLP_MODULE64 *)malloc(sizeof(IMAGEHLP_MODULE64));
	 mod->SizeOfStruct = sizeof(IMAGEHLP_MODULE64);

     process = GetCurrentProcess();

     SymSetOptions(SYMOPT_LOAD_LINES);
     SymInitialize( process, NULL, TRUE );

     frames               = CaptureStackBackTrace( 0, 100, stack, NULL );
     symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
     symbol->MaxNameLen   = 255;
     symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

     char *buf = (char *)malloc(1024);
     std::vector<std::string> vs;

     for( i = skip; i < frames; i++ ){
         SymFromAddr( process, ( DWORD64 )( stack[ i ] ), 0, symbol );

		 bool m64 = false;
		 BOOL mod_status = SymGetModuleInfo(process, symbol->Address, mod);
		 if (!mod_status) {
			 mod_status = SymGetModuleInfo64(process, symbol->Address, mod64);
			 m64 = true;
		 }

		 DWORD dwDisplacement;
		 bool l64 = false;

		 BOOL line_status = SymGetLineFromAddr(process, (ULONG64)stack[i], &dwDisplacement, line);
		 if (!line_status) {
			 line_status = SymGetLineFromAddr64(process, (ULONG64)stack[i], &dwDisplacement, line64);
			 l64 = true;
		 }

		 sprintf(buf, "%-3d %*p %s(%s +  0x%I64x)",
			 frames - i - 1, 
			 int(2 + sizeof(void*) * 2), stack[i],
			 mod_status ? (m64 ? mod64->ModuleName : mod->ModuleName) : "<unknown dll>",
			 symbol->Name,
			 symbol->Address);

		 vs.push_back(std::string(buf));

		 if (line_status) {
			 sprintf(buf, "\t\t%s:%d", l64 ? line64->FileName : line->FileName, l64 ? line64->LineNumber : line->LineNumber);
			 vs.push_back(std::string(buf));
		 }

     }

     free(buf);
     free(symbol);
	 SymCleanup(process);
     return vs;
}
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

         sprintf(buf, "%-3i 0x%0X %s", frames - i - 1, symbol->Address, symbol->Name );
         vs.push_back(std::string(buf));
     }

     free(buf);
     free(symbol);
     return vs;
}
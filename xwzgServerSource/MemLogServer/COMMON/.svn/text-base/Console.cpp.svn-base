#include "Console.h"
#include "BaseFunc.h"
#include <stdio.h>
#include <conio.h>

#ifdef	_DEBUG
#define ENABLE_CONSOLE
#endif

#if defined(ENABLE_CONSOLE)

void CON_Open()
{
	AllocConsole();
}

void CON_Printf(const char* format, ...)
{
	const int MAX_CHAR = 1024*8;
	char result[MAX_CHAR];

	//try{		
	DEBUG_TRY
		wvsprintf(result, format, (char*)(&format+1));	//thread safe
		_cprintf(result);
	DEBUG_CATCH("CATCH: *** ConsoleTrace() crash! ***\n");
	//}
	//catch(...){ LogSave("CATCH: *** ConsoleTrace() crash! ***\n"); }
}

#else

void CON_Open() {}
void CON_Printf(const char* format, ...) {}

#endif


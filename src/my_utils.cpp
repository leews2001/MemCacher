#include "my_utils.h"

#include <cstdarg>
#include <cstdio>
// comment out <DEBUG_FLAG> definition to disable verbose output
// #define DEBUG_FLAG


void verbose(const char* format, ...)
{
#ifdef DEBUG_FLAG
	va_list args;  // Used as a pointer to the next variable argument.

	va_start(args, format); // Initialize the pointer to arguments
	vprintf(format, args);
	va_end(args);
#endif

	return;
}
#ifndef _CORE_ASSERT_H_
#define _CORE_ASSERT_H_
#pragma once

#if DEBUG
#define assert(condition) ((void)0)
#else

#include <iostream>
#include <execinfo.h>

#define assert(condition) { static bool __ignore = false; if(!__ignore && !(condition)) __ignore = __assert_fn(#condition, __FILE__, __LINE__, __FUNCTION__); }

inline void __print_stack()
{
	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	for (i = 0; i < frames; ++i) {
	    printf("%s\n", strs[i]);
	}
	free(strs);
}

inline bool __assert_fn(const char *condition, const char *file, int line, const char *function)
{
	// open a dialog and ask end user whether to terminate, continue, ignore
	std::cerr << file << "(" << line << "): Assertion failed: (" << condition << "), function " << function << std::endl;
	__print_stack(); abort();
	return false;
}

#endif

#endif //_CORE_ASSERT_H_
#ifndef _CORE_CORE_H_
#define _CORE_CORE_H_
#pragma once

#define DEBUG 0
#define RELEASE 1

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define FILE_LINE_OUTPUT __FILE__ "(" STRINGIFY(__LINE__) "):"
#define FILE_LINE_OUTPUT2(file, line) file "(" STRINGIFY(line) "):"
#define FILE_LINE_OUTPUT_STREAM(file, line) file << "(" << line << "):"

#ifdef UNIT_TEST_VERBOSE
#define UNIT_TEST_T(x) std::cout << FILE_LINE_OUTPUT " Testing " #x ": " << (x ? "PASS" : "FAIL" ) << std::endl;
#define UNIT_TEST_F(x) std::cout << FILE_LINE_OUTPUT " Testing " #x ": " << (x ? "FAIL" : "PASS" ) << std::endl;
#else
#define UNIT_TEST_T(x) std::cout << ((x) ? "" : FILE_LINE_OUTPUT " Testing " #x ": FAIL\n");
#define UNIT_TEST_F(x) std::cout << ((x) ? FILE_LINE_OUTPUT " Testing " #x ": FAIL\n" : "");
#endif
#define UNIT_TEST(x) UNIT_TEST_T(x)

#define PRINT(x) std::cout << x << " ";
#define PRINTLN(x) std::cout << #x << std::endl;
#define PRINTX(x) std::cout << #x << " = " << x << std::endl;

#include <iostream>
#include "assert.h"
#include "timer.h"
#include "profiler.h"

#endif //_CORE_CORE_H_
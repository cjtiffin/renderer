#ifndef _CORE_CORE_H_
#define _CORE_CORE_H_
#pragma once

#define DEBUG 0
#define RELEASE 1

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define UNIT_TEST_VERBOSE
#ifdef UNIT_TEST_VERBOSE
#define UNIT_TEST_T(x) std::cout << STRINGIFY(__LINE__) " - Testing " #x ": " << (x ? "PASS" : "FAIL" ) << std::endl;
#define UNIT_TEST_F(x) std::cout << STRINGIFY(__LINE__) " - Testing " #x ": " << (x ? "FAIL" : "PASS" ) << std::endl;
#else
#define UNIT_TEST_T(x) std::cout << ((x) ? "" : STRINGIFY(__LINE__) " - Testing " #x ": FAIL\n");
#define UNIT_TEST_F(x) std::cout << ((x) ? STRINGIFY(__LINE__) " - Testing " #x ": FAIL\n" : "");
#endif

#define UNIT_TEST(x) UNIT_TEST_T(x)
#define PRINT(x) std::cout << x << std::endl;

#include <iostream>
#include "assert.h"
#include "timer.h"
#include "profiler.h"

#endif //_CORE_CORE_H_
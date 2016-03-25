#ifndef _CORE_ASSERT_H_
#define _CORE_ASSERT_H_
#pragma once

#if DEBUG
#define assert(condition) ((void)0)
#else
#include <cassert>
#endif

#endif //_CORE_ASSERT_H_
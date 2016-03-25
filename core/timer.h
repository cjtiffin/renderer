#ifndef _CORE_TIMER_H_
#define _CORE_TIMER_H_
#pragma once

namespace detail {
typedef double clock_tp;
typedef double clock_td;

#if _WIN32
// We want the most basic functionality that Windows.h gives us - specifically the performance timer stuff
// To get this, and not have windows.h mess up all of our code (SDK, min/max etc) define everything out
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#include <Windows.h>
inline double get_factor()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	if (frequency.QuadPart)
		return 1000.0 / static_cast<double>(frequency.QuadPart);
	return 0.0;
}
inline clock_tp now()
{
	static const double factor = get_factor();
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return static_cast<clock_tp>(static_cast<double>(currentTime.QuadPart) * factor);
}
inline clock_td duration(clock_tp const &tp)
{
	return tp; //already in milliseconds
}

#elif __APPLE__
#include <mach/mach_time.h>
inline double get_factor()
{
	mach_timebase_info_data_t MachInfo;
	mach_timebase_info(&MachInfo);
	if (MachInfo.denom)
		return static_cast<double>(MachInfo.numer) / MachInfo.denom;
	return 0.0;
}
inline clock_tp now()
{
	static const double factor = get_factor();
	return static_cast<clock_tp>(mach_absolute_time() * factor);
}
inline clock_td duration(clock_tp const &tp)
{
	return tp * 0.000001; //nanoseconds to milliseconds
}

#else
#error "Unsupported platform"
#endif
}

class Timer
{
public:
	typedef  detail::clock_tp  TimePoint;
	typedef  detail::clock_td  TimeDuration; // in ms

	static TimePoint Now()
	{
		return detail::now();
	}

	static TimeDuration Duration(TimePoint const &before)
	{
		return Duration(before, Now());
	}

	static TimeDuration Duration(TimePoint const &before, TimePoint const &after)
	{
		return detail::duration(after - before);
	}
};

#endif //_CORE_TIMER_H_
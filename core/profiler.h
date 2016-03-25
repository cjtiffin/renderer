#ifndef _CORE_PROFILER_H_
#define _CORE_PROFILER_H_
#pragma once

#include <unordered_map>
#include "Timer.h"

#define PROFILE_START(name)   Profiler::Get()->Start(#name);
#define PROFILE_END(name)     Profiler::Get()->End(#name);
#define PROFILE_SCOPED(name)  Profile _scoped_profile ## name(#name);
#define PROFILE_SCOPED_FN     Profile _scoped_profile(__FUNCTION__);

namespace detail {
inline void cout_fn(const char *text)
{
	std::cout << text;
}
}

class Profiler
{
public:
	~Profiler() { DumpStats(); }

	static Profiler *Get()
	{
		static Profiler instance;
		return &instance;
	}

	void Start(std::string const &name)
	{
		if (name.empty())
			return;

		ProfileData &p = all_profiles[name];
		p.times_called++;
		p.in_progress = true;
		p.start_time = Timer::Now();
	}

	void End(std::string const &name)
	{
		if (name.empty())
			return;

		ProfileData &p = all_profiles[name];
		p.in_progress = false;
		p.total_time += Timer::Duration(p.start_time);
	}

	void DumpStats(void (*ouput_fn)(const char *) = detail::cout_fn)
	{
		const char *separator     = "--------------------------------------------------------------------------------\n";
		const char *format_header = "%-35s %14s %14s %14s\n";
		const char *format_body   = "%-35s %14i %14.6f %14.6f\n";

		const unsigned max_buff = 1024;
		char buff[max_buff];

		// Formatting
		ouput_fn(separator);
		ouput_fn("Dumping profiler stats\n");
		ouput_fn(separator);

		snprintf(buff, max_buff, format_header, "Function Name", "Quantity", "Average", "Total");
		ouput_fn(buff);
		snprintf(buff, max_buff, format_header, "-------------", "--------", "-------", "-----");
		ouput_fn(buff);

		// Use an iterator to go through the profiles
		for (ProfileIter iter = all_profiles.begin(); iter != all_profiles.end(); ++iter)
		{
			ProfileData const &pd = iter->second;
			// Print each profiler call
			double avg = 0.0;
			if (pd.times_called > 0.0)
				avg = pd.total_time / pd.times_called;

			snprintf(buff, max_buff, format_body, iter->first.c_str(), pd.times_called, avg, pd.total_time);
			ouput_fn(buff);
		}

		// More formatting
		ouput_fn(separator);
		ouput_fn("\n");
	}

private:
	Profiler() {}

	class ProfileData
	{
	public:
		ProfileData()
			: times_called(0)
			, in_progress(0)
			, start_time(0)
			, total_time(0)
		{}

		unsigned             times_called;
		bool                 in_progress;
		Timer::TimePoint     start_time;
		Timer::TimeDuration  total_time;
	};

	typedef  std::unordered_map<std::string, ProfileData>  ProfileMap;
	typedef  ProfileMap::iterator                          ProfileIter;

	ProfileMap  all_profiles;
};

class Profile
{
public:
	Profile(std::string const &name) : m_name(name) { Profiler::Get()->Start(m_name); }
	~Profile() { Profiler::Get()->End(m_name); }

	std::string  m_name;
};

#endif //_CORE_PROFILER_H_
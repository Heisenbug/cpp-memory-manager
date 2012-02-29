#pragma once

#include <Windows.h>

class Timer
{
public:
	Timer();

public:
	void	Start();

	int	TimeElapsedSec() const;
	int	TimeElapsedMilliSec() const;
	LONGLONG TimeElapsedMicroSec() const;

private:
	LONGLONG start;
	LONGLONG freq;
};

inline Timer::Timer()
{
	Start();
}

inline void Timer::Start()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&start);
}

inline int Timer::TimeElapsedSec() const
{
	LONGLONG end;
	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	const LONGLONG diff = ((end - start) * 1) / freq;
	const unsigned int microseconds = (unsigned int)(diff & 0xffffffff);
}

inline int Timer::TimeElapsedMilliSec() const
{
	LONGLONG end;
	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	const LONGLONG diff = ((end - start) * 1000) / freq;
	const unsigned int microseconds = (unsigned int)(diff & 0xffffffff);
	return microseconds;
}

inline LONGLONG Timer::TimeElapsedMicroSec() const
{
	LONGLONG end;
	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	return ((end - start) * 1000000) / freq;
}

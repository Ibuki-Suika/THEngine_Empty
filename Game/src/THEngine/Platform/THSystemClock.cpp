#include "THSystemClock.h"
#include <Common\THCommon.h>

namespace THEngine
{
	SystemClock::SystemClock()
	{
		LARGE_INTEGER intFrequency;
		QueryPerformanceFrequency(&intFrequency);
		this->frequency = intFrequency.QuadPart;
	}

	Time SystemClock::GetTime()
	{
		LARGE_INTEGER intCurrentTime;
		QueryPerformanceCounter(&intCurrentTime);
		long long microsecond = (double)intCurrentTime.QuadPart / frequency * 1000000;
		return Time(microsecond);
	}
}
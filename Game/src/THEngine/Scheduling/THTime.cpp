#include "THTime.h"
#include <chrono>

namespace THEngine
{
	void Time::Set(int hour, int minute, int second, int millisecond, int microsecond)
	{
		long long minuteNow = hour * 60 + minute;
		long long secondNow = minuteNow * 60 + second;
		long long millisecondNow = secondNow * 1000 + millisecond;
		this->microsecond = millisecondNow * 1000 + microsecond;
	}
}
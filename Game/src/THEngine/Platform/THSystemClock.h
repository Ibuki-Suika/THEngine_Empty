#ifndef TH_SYSTEM_CLOCK_H
#define TH_SYSTEM_CLOKC_H

#include <Common\THObject.h>
#include <Scheduling\THTime.h>

namespace THEngine
{
	class SystemClock : public Object
	{
	private:
		long long frequency = 0;

	private:
		SystemClock();

	public:
		static SystemClock* GetInstance();
		Time GetTime();
	};
}

#endif

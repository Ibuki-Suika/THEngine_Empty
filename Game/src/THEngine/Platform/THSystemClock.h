#ifndef TH_SYSTEM_CLOCK_H
#define TH_SYSTEM_CLOKC_H

#include <Common\THObject.h>
#include <Common\THTemplates.h>
#include <Scheduling\THTime.h>

namespace THEngine
{
	class SystemClock : public Object, public Singleton<SystemClock>
	{
		friend class Singleton<SystemClock>;

	private:
		long long frequency = 0;

	private:
		SystemClock();

	public:
		Time GetTime();
	};
}

#endif

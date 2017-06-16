#ifndef THSCHEDULER_H
#define THSCHEDULER_H

#include <Common\THObject.h>
#include <Common\THList.h>
#include "THFrameTimer.h"
#include "THTimer.h"

namespace THEngine
{
	class Scheduler : public Object
	{
	private:
		LinkedList<FrameTimer*> frameTimerList;
		LinkedList<Timer*> timerList;

		using TimerFunc = std::function<void()>;

	private:
		void UpdateTimer();
		void UpdateFrameTimer();

	public:
		Scheduler();
		Scheduler(const Scheduler& scheduler);
		virtual ~Scheduler();

		void Update();

		inline void AddTimer(Timer* timer) { timerList.Add(timer); }
		inline void AddFrameTimer(FrameTimer* timer) { frameTimerList.Add(timer); }
		void AddFrameTimer(int delay, TimerFunc run);
	};
}

#endif
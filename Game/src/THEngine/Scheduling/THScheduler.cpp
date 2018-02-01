#include "THScheduler.h"
#include <Common\THCommon.h>

namespace THEngine
{
	Scheduler::Scheduler()
	{
	}

	Scheduler::Scheduler(const Scheduler& scheduler) : Object(scheduler)
	{
		auto iter = const_cast<Scheduler&>(scheduler).frameTimerList.GetIterator();
		while (iter->HasNext())
		{
			auto timer = iter->Next();
			AddFrameTimer((FrameTimer*)timer->Clone().Get());
		}

		auto iter2 = const_cast<Scheduler&>(scheduler).timerList.GetIterator();
		while (iter2->HasNext())
		{
			auto timer = iter->Next();
			AddTimer((Timer*)timer->Clone().Get());
		}
	}

	Scheduler::~Scheduler()
	{
	}

	void Scheduler::Update()
	{
		UpdateFrameTimer();
		UpdateTimer();
	}

	void Scheduler::UpdateFrameTimer()
	{
		auto iter = frameTimerList.GetIterator();
		while (iter->HasNext())
		{
			auto timer = iter->Next();

			if (timer->IsPaused() == false)
			{
				timer->frameLeft--;
				if (timer->frameLeft == 0)
				{
					timer->run();
					iter->Remove();
				}
			}
		}
	}

	void Scheduler::UpdateTimer()
	{
		//TODO : Need to be implemented later

		/*auto iter = timerList.GetIterator();
		while (iter->HasNext())
		{
			auto timer = iter->Next();

			if (timer->IsPaused() == false)
			{
				timer->frameLeft--;
				if (timer->frameLeft == 0)
				{
					timer->run();
					iter->Remove();
				}
			}
		}*/
	}

	void Scheduler::AddFrameTimer(int delay, TimerFunc run)
	{
		Ptr<FrameTimer> timer = Ptr<FrameTimer>::New();
		timer->SetFrame(delay);
		timer->run = run;
		AddFrameTimer(timer);
	}
}
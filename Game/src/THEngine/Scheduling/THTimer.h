#ifndef TH_TIMER_H
#define TH_TIMER_H

#include "THTime.h"
#include <Common\THObject.h>
#include <Common\THPtr.h>
#include <functional>

namespace THEngine
{
	class Timer : public Object
	{
	private:
		Time time;
		Time lastTimeStamp;
		bool started = false;
		bool paused = false;

	public:
		std::function<void()> run = []() {};

	public:
		Timer() = default;
		virtual ~Timer() = default;

		virtual Ptr<Object> Clone() const override;

		inline void Pause() { paused = true; }
		inline void Resume() { paused = false; }
		inline bool IsPaused() { return paused; }

		inline void SetTime(const Time& time) { this->time = time; }
	};
}

#endif

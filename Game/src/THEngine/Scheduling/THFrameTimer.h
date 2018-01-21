#ifndef THFRAMETIMER_H
#define THFRAMETIMER_H

#include <Common\THObject.h>
#include <Common\THPtr.h>
#include <functional>

namespace THEngine
{
	class FrameTimer : public Object
	{
	private:
		int frameLeft;
		bool paused;

	public:
		FrameTimer();
		virtual ~FrameTimer();

		std::function<void()> run = []() {};

		virtual Ptr<Object> Clone() const override;

		inline void Pause() { paused = true; }
		inline void Resume() { paused = false; }
		inline bool IsPaused() { return paused; }

		inline void SetFrame(int frame) { frameLeft = frame; }

		friend class Scheduler;
	};
}

#endif
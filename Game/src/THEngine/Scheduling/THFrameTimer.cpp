#include "THFrameTimer.h"

namespace THEngine
{
	FrameTimer::FrameTimer()
	{
		frameLeft = 0;
		paused = false;
	}

	FrameTimer::~FrameTimer()
	{
	}

	Ptr<Object> FrameTimer::Clone() const
	{
		return Ptr<FrameTimer>::New(*this).Get();
	}
}
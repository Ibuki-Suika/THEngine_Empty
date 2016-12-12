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

	Object* FrameTimer::Clone()
	{
		return new FrameTimer(*this);
	}
}
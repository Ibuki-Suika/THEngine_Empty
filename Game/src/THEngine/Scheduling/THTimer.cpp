#include "THTimer.h"

namespace THEngine
{
	Ptr<Object> Timer::Clone() const
	{
		return Ptr<Timer>::New(*this).Get();
	}
}
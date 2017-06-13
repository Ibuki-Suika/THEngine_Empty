#include "THTimer.h"

namespace THEngine
{
	Object* Timer::Clone()
	{
		return new Timer(*this);
	}
}
#include "THDataStack.h"

namespace THEngine
{
	DataStack::DataStack()
	{
	}

	DataStack::~DataStack()
	{

	}

	void DataStack::Reset()
	{
		position = Vector3f(0, 0, 0);
	}
}
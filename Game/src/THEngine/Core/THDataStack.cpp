#include "THDataStack.h"

namespace THEngine
{
	DataStack* DataStack::instance = nullptr;

	DataStack::DataStack()
	{
		ASSERT(instance == nullptr);
		instance = this;
	}

	DataStack::~DataStack()
	{

	}

	DataStack* DataStack::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new DataStack();
		}
		return instance;
	}

	void DataStack::Reset()
	{
		position = Vector3f(0, 0, 0);
	}
}
#include "THException.h"

namespace THEngine
{
	Exception::Exception()
	{

	}

	Exception::Exception(String info)
	{
		this->info = info;
	}

	Exception::~Exception()
	{

	}
}
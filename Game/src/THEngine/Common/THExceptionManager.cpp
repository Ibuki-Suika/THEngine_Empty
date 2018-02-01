#include "THExceptionManager.h"
#include "THDefine.h"

namespace THEngine
{
	ExceptionManager::~ExceptionManager()
	{
	}

	void ExceptionManager::PushException(Ptr<Exception> exception)
	{
		this->exception = exception;
	}

	void ExceptionManager::SetHandled()
	{
		exception = Ptr<Exception>::New();
	}
}
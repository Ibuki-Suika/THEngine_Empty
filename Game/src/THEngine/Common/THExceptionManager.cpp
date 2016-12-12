#include "THExceptionManager.h"
#include "THDefine.h"

namespace THEngine
{
	ExceptionManager* ExceptionManager::instance = nullptr;

	ExceptionManager::ExceptionManager()
	{
		exception = new Exception();
		exception->Retain();
	}

	ExceptionManager::~ExceptionManager()
	{
		TH_SAFE_RELEASE(exception);
	}

	ExceptionManager* ExceptionManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new ExceptionManager();
		}
		return instance;
	}

	void ExceptionManager::PushException(Exception* exception)
	{
		TH_SAFE_RELEASE(this->exception);
		this->exception = exception;
		this->exception->Retain();
	}

	void ExceptionManager::SetHandled()
	{
		TH_SAFE_RELEASE(exception);
		exception = new Exception();
		exception->Retain();
	}
}
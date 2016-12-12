#ifndef THEXCEPTIONMANAGER_H
#define THEXCEPTIONMANAGER_H

#include "THException.h"

namespace THEngine
{
	class ExceptionManager : public Object
	{
	private:
		Exception* exception = nullptr;

		static ExceptionManager* instance;

	private:
		ExceptionManager();

	public:
		virtual ~ExceptionManager();

		static ExceptionManager* GetInstance();

		inline Exception* GetException() { return exception; }

		void SetHandled();

		void PushException(Exception* exception);
	};
}

#endif
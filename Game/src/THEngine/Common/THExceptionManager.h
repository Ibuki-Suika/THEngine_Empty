#ifndef THEXCEPTIONMANAGER_H
#define THEXCEPTIONMANAGER_H

#include "THException.h"
#include "THTemplates.h"

namespace THEngine
{
	class ExceptionManager : public Object, public Singleton<ExceptionManager>
	{
		friend class Singleton<ExceptionManager>;
	private:
		Ptr<Exception> exception = Ptr<Exception>::New();

	private:
		ExceptionManager() = default;

	public:
		virtual ~ExceptionManager();
		inline Ptr<Exception> GetException() { return exception; }

		void SetHandled();

		void PushException(Ptr<Exception> exception);
	};
}

#endif
#ifndef THEXCEPTION_H
#define THEXCEPTION_H

#include "THObject.h"
#include "THString.h"

namespace THEngine
{
	class Exception : public Object
	{
	protected:
		String info = "Unknown Exception";

	public:
		Exception();
		Exception(String info);
		virtual ~Exception();

		inline void SetInfo(const String& info) { this->info = info; }
		inline String GetInfo() { return info; }
	};
}

#endif
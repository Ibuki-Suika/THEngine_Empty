#ifndef THOBJECT_H
#define THOBJECT_H

#include <atomic>

namespace THEngine
{
	class IKeyboardListener;

	class Object
	{
	private:
		std::atomic_int refCount;

	public:
		Object();
		Object(const Object& object);
		virtual ~Object();

		virtual Object* Clone();

		Object& operator=(const Object& obj);

		void Retain();
		void Release();
	};
}

#endif
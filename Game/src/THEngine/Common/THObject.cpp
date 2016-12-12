#include "THObject.h"
#include "THDefine.h"
#include <typeinfo>

namespace THEngine
{
	Object::Object()
	{
		refCount = 0;
	}

	Object::Object(const Object& object)
	{
		refCount = 0;
	}

	Object::~Object()
	{

	}

	Object* Object::Clone()
	{
		return nullptr;
	}

	void Object::Retain()
	{
		refCount++;
	}

	void Object::Release()
	{
		ASSERT(refCount > 0);
		refCount--;
		if (refCount == 0)
		{
			delete this;
		}
	}
}


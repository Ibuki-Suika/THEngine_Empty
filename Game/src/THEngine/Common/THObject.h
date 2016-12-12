#ifndef THOBJECT_H
#define THOBJECT_H


namespace THEngine
{
	class IKeyboardListener;

	class Object
	{
	private:
		int refCount;

	public:
		Object();
		Object(const Object& object);
		virtual ~Object();

		virtual Object* Clone();
		
		virtual void Retain() final;
		virtual void Release() final;
	};
}

#endif
#ifndef THOBJECT_H
#define THOBJECT_H

#include <atomic>

namespace THEngine
{
	class IKeyboardListener;

	template<class T>
	class Ptr;

	class Object
	{
	public:
		virtual Ptr<Object> Clone() const;

		Object& operator=(const Object& obj);
		Object& operator=(Object&& rhs) = default;

		void* operator new(size_t count, void* ptr) { return ::operator new(count, ptr); }
		void* operator new[](size_t count, void* ptr) { return ::operator new[](count, ptr); }
		void operator delete(void* ptr, void* place) { return ::operator delete(ptr, place); }
		void operator delete[](void* ptr, void* place) { return ::operator delete(ptr, place); }

	protected:
		Object() = default;
		Object(const Object& object);
		Object(Object&& rhs) = default;
		virtual ~Object();

	private:
		void* operator new(size_t size) { return ::operator new(size); }
		void* operator new[](size_t size) { return ::operator new[](size); }
		void operator delete(void* data) { ::operator delete(data); }
		void operator delete[](void* data) { ::operator delete[](data); }

		void Retain();
		void Release();

	private:
		std::atomic_int refCount_ = 1;    //when constructed, raw pointer owns a reference
		bool alive = true;

		template<class T>
		friend class Ptr;
	};
}

#endif
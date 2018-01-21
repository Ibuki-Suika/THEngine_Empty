#ifndef TH_PTR_H
#define TH_PTR_H

#include "THObject.h"

namespace THEngine
{
	template<class T>
	class Ptr
	{
	public:
		Ptr() = default;
		Ptr(T* obj) { Assign(obj); }
		Ptr(const Ptr<T>& rhs) { Assign(rhs.obj_); }
		Ptr(Ptr&& rhs) { Assign(rhs.obj_); }

		~Ptr() { Destroy(obj_); }

		Ptr& operator=(const Ptr<T>& rhs)
		{
			Reset(rhs.obj_);
			return *this;
		}

		Ptr& operator=(Ptr<T>&& rhs)
		{
			Reset(rhs.obj_);
			return *this;
		}

		Ptr& operator=(T* obj)
		{
			Reset(obj);
			return *this;
		}

		T* operator->() const { return obj_; }
		T* Get() const { return obj_; }

		bool operator ==(const T* p) { return obj_ == p; }
		bool operator !=(const T* p) { return obj_ != p; }
		bool operator ==(const Ptr<T>& rhs) { return obj_ == rhs.obj_; }
		bool operator !=(const Ptr<T>& rhs) { return obj_ != rhs.obj_; }

		operator bool() const { return obj_ != nullptr; }

		void Reset(T* rhs)
		{
			if (rhs == obj_)
				return;
			T* temp = obj_;
			Assign(rhs);
			Destroy(temp);
		}

		/* used for manually constructing smart pointers like this (because every new Object will have refCount = 1)
		 * T* obj = malloc(sizeof(T));
		 * new(obj) T();
		 * Ptr<T> p = Create_NoRetain(obj);

		 Note that this should only be used in cases like singleton or factory method, in which cases you need to declare constructor as private.
		 In any other case, you should always use Ptr<T>::New() to create new objects.
		*/
		static Ptr<T> Create_NoRetain(T* rhs)
		{
			Ptr ret;
			ret.obj_ = rhs;
			return ret;
		}

		template<class... Args>
		static Ptr New(Args... args)
		{
			Ptr ret(new T(std::forward<Args>(args)...));
			Object* obj = reinterpret_cast<Object*>(ret.obj_);
			obj->Release();   //destroy the reference owned by the raw pointer
			return ret;
		}

	private:
		void Destroy(T* obj)
		{
			if (obj)
			{
				Object* o = reinterpret_cast<Object*>(obj);
				if (o->alive)
					o->Release();
			}
		}

		void Assign(T* obj)
		{
			obj_ = obj;
			if (obj_)
			{
				Object* o = reinterpret_cast<Object*>(obj_);
				if (o->alive)
					o->Retain();
			}
		}

	private:
		T* obj_ = nullptr;
	};
}

#endif

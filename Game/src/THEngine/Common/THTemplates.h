#ifndef TH_TEMPLATES_H
#define TH_TEMPLATES_H

#include "THPtr.h"

namespace THEngine
{
	class NonClonable
	{
	public:
		NonClonable() = default;

	private:
		NonClonable(const NonClonable& rhs) = delete;
		NonClonable(NonClonable&& rhs) = delete;
		NonClonable& operator=(const NonClonable& rhs) = delete;
		NonClonable& operator =(NonClonable&& rhs) = delete;
	};

	template<class T>
	class Singleton : public NonClonable
	{
	private:
		static Ptr<T>& _GetInstance()
		{
			static Ptr<T> instance = Ptr<T>::Create_NoRetain(_CreateInstance());
			return instance;
		}

		static T* _CreateInstance()
		{
			T* inst = (T*)malloc(sizeof(T));
			new(inst) T();
			return inst;
		}

	public:
		static Ptr<T> GetInstance()
		{
			return _GetInstance();
		}

		static void DestroyInstance()
		{
			_GetInstance() = nullptr;
		}

	protected:
		Singleton() = default;
	};
}

#endif

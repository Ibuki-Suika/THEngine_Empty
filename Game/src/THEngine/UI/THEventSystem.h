#ifndef THEVENTSYSTEM_H
#define THEVENTSYSTEM_H

#include "../Common/THCommon.h"
#include "THEvents.h"
#include <list>

namespace THEngine
{
	class EventSystem : public Object
	{
	private:
		EventSystem();
		static EventSystem* instance;

	private:
		template<class T>
		class ListenerInfo : public Object
		{
		public:
			T listener;
			bool needRemove = false;
		};


	protected:
		LinkedList<ListenerInfo<IKeyDownListener*>*> keyDownListeners;

	public:
		~EventSystem();

		static EventSystem* Create();

		static EventSystem* GetInstance();

		void Update();

		void OnKeyDown(int key);

		void RegisterKeyDownListener(IKeyDownListener* listener);
		void UnRegisterKeyDownListener(IKeyDownListener* listener);
	};
}

#endif
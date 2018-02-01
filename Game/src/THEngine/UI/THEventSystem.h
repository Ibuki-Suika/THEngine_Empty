#ifndef TH_EVENT_SYSTEM_H
#define TH_EVENT_SYSTEM_H

#include <Common\THCommon.h>

namespace THEngine
{
	class IKeyDownListener;

	class EventSystem : public Object, public Singleton<EventSystem>
	{
		friend class Singleton<EventSystem>;
	private:
		EventSystem();

	private:
		template<class T>
		class ListenerInfo : public Object
		{
		public:
			T listener;
			bool needRemove = false;
		};

	protected:
		LinkedList<Ptr<ListenerInfo<IKeyDownListener*>>> keyDownListeners;

	public:
		~EventSystem();

		bool Init();

		void Update();

		void OnKeyDown(int key);

		void RegisterKeyDownListener(IKeyDownListener* listener);
		void UnRegisterKeyDownListener(IKeyDownListener* listener);
	};
}

#endif
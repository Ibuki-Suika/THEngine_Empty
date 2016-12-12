#ifndef THENGINEOBJECT_H
#define THENGINEOBJECT_H

#include "THObject.h"
#include "THScheduler.h"

namespace THEngine
{
	class EngineObject : public Object
	{
	protected:
		Scheduler* scheduler = nullptr;
		bool paused = false;

	public:
		EngineObject();
		EngineObject(const EngineObject& engineObject);
		virtual ~EngineObject();

		virtual void Update() = 0;
		virtual void Draw() = 0;

		inline Scheduler* GetScheduler() { return scheduler; }

		inline bool IsPaused() { return paused; }
		inline void Pause() { paused = true; }
		inline void Resume() { paused = false; }
	};
}

#endif
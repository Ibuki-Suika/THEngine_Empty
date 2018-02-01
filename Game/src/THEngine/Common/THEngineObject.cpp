#include "THEngineObject.h"
#include "THDefine.h"
#include <Scheduling\THScheduler.h>

namespace THEngine
{
	EngineObject::EngineObject()
	{
		scheduler = Ptr<Scheduler>::New();
	}

	EngineObject::EngineObject(const EngineObject& engineObject) : Object(engineObject)
	{
		paused = engineObject.paused;
		scheduler = Ptr<Scheduler>::New(*engineObject.scheduler.Get());
	}

	EngineObject::~EngineObject()
	{
	}

	void EngineObject::Update()
	{
		if (started == false)
		{
			started = true;
			OnStart();
		}
		scheduler->Update();
	}

	void EngineObject::OnStart()
	{
	}

	void EngineObject::OnLoad(Ptr<AsyncInfo> info)
	{
		if (loaded)
		{
			return;
		}
		loaded = true;
	}
}
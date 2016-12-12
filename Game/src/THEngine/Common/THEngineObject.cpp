#include "THEngineObject.h"
#include "THDefine.h"

namespace THEngine
{
	EngineObject::EngineObject()
	{
		scheduler = new Scheduler();
		scheduler->Retain();
	}

	EngineObject::EngineObject(const EngineObject& engineObject) : Object(engineObject)
	{
		paused = engineObject.paused;
		scheduler = new Scheduler(*engineObject.scheduler);
		scheduler->Retain();
	}

	EngineObject::~EngineObject()
	{
		TH_SAFE_RELEASE(scheduler);
	}

	void EngineObject::Update()
	{
		scheduler->Update();
	}
}
#include "THEventSystem.h"
#include "THEvents.h"

using namespace THEngine;

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem()
{
}

bool EventSystem::Init()
{
	return true;
}

void EventSystem::Update()
{
	auto iter = keyDownListeners.GetIterator();
	while (iter->HasNext())
	{
		auto info = iter->Next();
		if (info->needRemove)
		{
			iter->Remove();
		}
	}
}

void EventSystem::OnKeyDown(int key)
{
	auto iter = keyDownListeners.GetIterator();
	while (iter->HasNext())
	{
		auto info = iter->Next();
		if (info->listener->OnKeyDown(nullptr, key))
		{
			break;
		}
	}
}

void EventSystem::RegisterKeyDownListener(IKeyDownListener* listener)
{
	auto iter = keyDownListeners.GetIterator();
	while (iter->HasNext())
	{
		auto info = iter->Next();
		if (info->listener == listener)
		{
			return;
		}
	}
	auto info = Ptr<ListenerInfo<IKeyDownListener*>>::New();
	info->listener = listener;
	keyDownListeners.Add(info);
}

void EventSystem::UnRegisterKeyDownListener(IKeyDownListener* listener)
{
	auto iter = keyDownListeners.GetIterator();
	while (iter->HasNext())
	{
		auto info = iter->Next();
		if (info->listener == listener)
		{
			iter->Remove();
			return;
		}
	}
}
#include "THEventSystem.h"
#include "THEvents.h"

using namespace THEngine;

EventSystem* EventSystem::instance = nullptr;

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem()
{
}

EventSystem* EventSystem::Create()
{
	if (instance == nullptr)
	{
		instance = new EventSystem();
		return instance;
	}
	return nullptr;
}

EventSystem* EventSystem::GetInstance()
{
	return instance;
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
	auto info = new ListenerInfo<IKeyDownListener*>();
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
#include "THMenu.h"
#include "THEventSystem.h"
#include <Platform\THSound.h>

namespace THEngine
{
	Menu::Menu()
	{
	}

	Menu::Menu(int left, int top, int width, int height) : Layer(left, top, width, height)
	{
	}

	Menu::~Menu()
	{
	}

	void Menu::Update()
	{
		Layer::Update();

		auto iter = this->itemList.GetIterator();
		int i = 0;
		while (iter->HasNext())
		{
			auto item = iter->Next();
			if (item->NeedRemove())
			{
				iter->Remove();
				if (i <= currentSelection)
				{
					currentSelection--;
				}
			}
			i++;
		}
	}

	void Menu::AddMenuItem(MenuItem* menuItem)
	{
		itemList.Add(menuItem);
		menuItem->menu = this;
		AddChild(menuItem);

		if (currentSelection < 0)
		{
			if (menuItem->selectable)
			{
				Select(itemList.Size() - 1, false);
			}
		}
	}

	void Menu::OnStart()
	{
		Layer::OnStart();
		EventSystem::GetInstance()->RegisterKeyDownListener(this);
	}

	void Menu::OnDestroy()
	{
		Layer::OnDestroy();
		EventSystem::GetInstance()->UnRegisterKeyDownListener(this);
	}

	void Menu::OnMenuItemClicked(int item)
	{
	}

	void Menu::Select(int index, bool playSound)
	{
		if (currentSelection >= 0)
		{
			itemList.Get(currentSelection)->OnDeselect();
		}
		currentSelection = index;
		itemList.Get(index)->OnSelect();

		if (playSound)
		{
			if (soundSelect)
			{
				soundSelect->Play();
			}
		}
	}

	void Menu::Click(int index)
	{
		if (index >= itemList.Size() || index < 0)
		{
			return;
		}
		itemList.Get(index)->OnClick();
		OnMenuItemClicked(index);

		if (soundOK)
		{
			soundOK->Play();
		}
	}

	bool Menu::OnKeyDown(EngineObject* sender, int key)
	{
		switch (key)
		{
		case VK_DOWN:
			for (int i = currentSelection + 1; i < itemList.Size(); i++)
			{
				if (itemList.Get(i)->selectable)
				{
					Select(i);
					return true;
				}
			}
			break;
		case VK_UP:
			for (int i = currentSelection - 1; i >= 0; i--)
			{
				if (itemList.Get(i)->selectable)
				{
					Select(i);
					return true;
				}
			}
			break;
		case VK_RETURN:
		case 'z':
		case 'Z':
			Click(currentSelection);
			return true;
		}

		return false;
	}
}
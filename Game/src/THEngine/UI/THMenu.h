#ifndef TH_MENU_H
#define TH_MENU_H

#include <Common\THCommon.h>
#include "THEvents.h"
#include "THMenuItem.h"
#include <Core\THLayer.h>

namespace THEngine
{
	class Sound;

	class Menu : public Layer, public IKeyDownListener
	{
	protected:
		ArrayList<MenuItem*> itemList;

		int currentSelection = -1;

		Sound* soundSelect = nullptr;
		Sound* soundOK = nullptr;

	public:
		Menu();
		Menu(int left, int top, int width, int height);
		virtual ~Menu();

		virtual void Update() override;
		virtual bool OnKeyDown(EngineObject* sender, int key) override;

		virtual void OnStart() override;
		virtual void OnDestroy() override;

		virtual void OnMenuItemClicked(int item);

		inline void Select(int index) { Select(index, true); }

		void Click(int index);

		void Select(int index, bool playSound);

		void SetSelection(int index);

		inline void SetSoundSelect(Sound* sound) { soundSelect = sound; }
		inline void SetSoundOK(Sound* sound) { soundOK = sound; }

		inline int GetItemCount() { return itemList.Size(); }
		inline MenuItem* GetMenuItem(int index) { return itemList.Get(index); }
		inline ArrayList<MenuItem*>* GetMenuItems() { return &this->itemList; }

		void AddMenuItem(MenuItem* menuItem);

		inline void ClearItems()
		{
			itemList.Clear();
			currentSelection = -1;
		}
	};
}

#endif
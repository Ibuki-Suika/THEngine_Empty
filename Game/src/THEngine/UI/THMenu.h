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
		ArrayList<Ptr<MenuItem>> itemList;

		int currentSelection = -1;

		Ptr<Sound> soundSelect;
		Ptr<Sound> soundOK;

	public:
		Menu();
		Menu(int left, int top, int width, int height);
		virtual ~Menu();

		virtual void Update() override;
		virtual bool OnKeyDown(Ptr<EngineObject> sender, int key) override;

		virtual void OnStart() override;
		virtual void OnDestroy() override;

		virtual void OnMenuItemClicked(int item);

		inline void Select(int index) { Select(index, true); }

		void Click(int index);

		void Select(int index, bool playSound);

		inline void SetSoundSelect(Ptr<Sound> sound) { soundSelect = sound; }
		inline void SetSoundOK(Ptr<Sound> sound) { soundOK = sound; }

		inline int GetItemCount() { return itemList.Size(); }
		inline Ptr<MenuItem> GetMenuItem(int index) { return itemList.Get(index); }
		inline ArrayList<Ptr<MenuItem>>* GetMenuItems() { return &this->itemList; }

		void AddMenuItem(Ptr<MenuItem> menuItem);

		inline void ClearItems()
		{
			itemList.Clear();
			currentSelection = -1;
		}
	};
}

#endif
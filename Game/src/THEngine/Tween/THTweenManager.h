#ifndef THTWEENMANAGER_H
#define THTWEENMANAGER_H

#include <Common\THCommon.h>
#include "THTween.h"

namespace THEngine
{
	class TweenManager : public Object
	{
	private:
		LinkedList<Ptr<Tween>> tweenList;
		
	public:
		TweenManager();
		TweenManager(const TweenManager& tweenManager);
		virtual ~TweenManager();

		void Update();

		inline void AddTween(Ptr<Tween> tween) { tweenList.Add(tween); }

		inline void KillTween(Ptr<Tween> tween) { tweenList.Remove(tween); }

		inline void ClearTweens() { tweenList.Clear(); }
	};
}

#endif
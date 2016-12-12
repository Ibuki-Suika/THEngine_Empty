#ifndef THTWEENMANAGER_H
#define THTWEENMANAGER_H

#include "../Common/THCommon.h"
#include "THTween.h"

namespace THEngine
{
	class Tween;

	class TweenManager : public Object
	{
	private:
		LinkedList<Tween*> tweenList;
		
	public:
		TweenManager();
		TweenManager(const TweenManager& tweenManager);
		virtual ~TweenManager();

		void Update();

		inline void AddTween(Tween* tween) { tweenList.Add(tween); }

		inline void KillTween(Tween* tween) { tweenList.Remove(tween); }
	};
}

#endif
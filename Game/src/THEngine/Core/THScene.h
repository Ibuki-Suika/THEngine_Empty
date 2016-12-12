#ifndef THSCENE_H
#define THSCENE_H

#include "../Common/THCommon.h"
#include "THLayer.h"

namespace THEngine
{
	class Scene : public EngineObject
	{
	protected:
		LinkedList<Layer*> layers;

	public:
		virtual void Update() override;
		virtual void Draw() override;

		virtual void OnLoad();
		virtual void OnSceneChanged();

		void AddLayer(Layer* layer);
	};
}

#endif
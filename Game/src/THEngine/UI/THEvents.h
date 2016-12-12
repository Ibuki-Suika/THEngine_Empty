#ifndef THEVENTS_H
#define THEVENTS_H

namespace THEngine
{
	class IKeyDownListener
	{
	public:
		virtual bool OnKeyDown(EngineObject* sender, int key) = 0;
	};
}

#endif
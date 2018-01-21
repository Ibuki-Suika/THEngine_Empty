#ifndef TH_DATA_STACK_H
#define TH_DATA_STACK_H

#include <Common\THCommon.h>
#include <Math\THVector.h>

namespace THEngine
{
	class DataStack : public Object, public Singleton<DataStack>
	{
		friend class Singleton<DataStack>;
	private:
		Vector3f position;

	private:
		DataStack();

		void Reset();

	public:
		virtual ~DataStack();

		friend class Game;
		friend class Layer;
		friend class GameObject;
	};
}

#endif
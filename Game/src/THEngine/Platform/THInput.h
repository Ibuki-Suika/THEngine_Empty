#ifndef THINPUT_H
#define THINPUT_H

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION   0x0800
#endif

#include <dinput.h>
#include "../Common/THCommon.h"
#include "THApplication.h"


#define DIKEYDOWN(data,id) (data[id] & 0x80)

namespace THEngine
{
	class Input : public Object
	{
	private:
		static Input* instance;

		IDirectInput8* input;
		IDirectInputDevice8* keyboard;

		BYTE keystate[256];

	private:
		int GetKeyboardState();

	public:
		Input();
		virtual ~Input();

		static Input* GetInstance();
		static Input* Create(Application* app);

		//刷新键盘、输入设备的状态
		void Update();

		bool KeyDown(int key);
	};
}

#endif
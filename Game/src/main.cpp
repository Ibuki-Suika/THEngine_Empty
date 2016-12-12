// Game.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Resource.h"
#include <THEngine.h>

using namespace THEngine;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	Game game;
	if (game.CreateGame(640, 480, false, "test", IDI_GAME, IDI_SMALL) == false)
	{
		auto exceptionManager = ExceptionManager::GetInstance();
		auto exception = exceptionManager->GetException();
		THMessageBox((String)"��ʼ��ʧ�ܣ�ԭ���ǣ�\n" + exception->GetInfo());
		return 0;
	}

	return game.Run();
}
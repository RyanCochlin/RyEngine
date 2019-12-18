//Ryan Cochlin - 7/13/2019
//Assertions Main
//TODO document

#include <Windows.h>

#include "Core.h"
#include "SubSystems/SubSystemManager.h"

namespace RyEngine
{
	void RyEngineCore::Initialize()
	{
		SubSystemManager::Instance().RegisterSubSystems();

		//TODO for now just gonna start the subsystems here
		SubSystemManager::Instance().SpinUpSubSystems();
	}

	void RyEngineCore::Run()
	{
		MSG msg = {};

		while (msg.message != WM_QUIT)
		{
			//TODO: Handle input
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				SubSystemManager::Instance().OnRender();
			}
		}
	}

	void RyEngineCore::Release()
	{
		SubSystemManager::Instance().Release();
	}
}
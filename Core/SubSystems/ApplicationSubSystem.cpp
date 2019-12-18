#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <Windows.h>

#include "ApplicationSubSystem.h"

namespace RyEngine
{
	void ApplicationSubSystem::OnRegister()
	{
		_hInstance = (HINSTANCE)GetModuleHandle(NULL);
	}
	
	void ApplicationSubSystem::OnStart(){}
	void ApplicationSubSystem::Release(){}
}
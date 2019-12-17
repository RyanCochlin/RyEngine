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

	void RyEngineCore::CoreMain()
	{
		while (true)
		{

		}
	}
}
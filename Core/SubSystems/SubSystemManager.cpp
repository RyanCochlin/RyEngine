#include <Windows.h>

#include "Core.h"
#include "SubSystemManager.h"
#include "Platform/Window/WindowSystem.h"

namespace RyEngine
{
	WindowSystem SubSystemManager::windowSystem()
	{
		return *_windowSystem;
	}

	void SubSystemManager::RegisterSubSystems()
	{
		AddAllSubSystems();
		
		for (size_t i = 0; i < _allSubSystems.size(); i++)
		{
			Register(_allSubSystems[i]);
		}
	}

	void SubSystemManager::SpinUpSubSystems()
	{
		for (size_t i = 0; i < _allSubSystems.size(); i++)
		{
			_allSubSystems[i]->OnStart();
		}
	}

	void SubSystemManager::Add(SubSystem* s)
	{
		_allSubSystems.push_back(s);
	}

	void SubSystemManager::AddAllSubSystems()
	{
		_windowSystem = new WindowSystem();
		Add(_windowSystem);
	}

	void SubSystemManager::Register(SubSystem* s)
	{
		s->OnRegister();
	}
}
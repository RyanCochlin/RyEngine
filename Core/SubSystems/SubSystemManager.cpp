#include <Windows.h>

#include "Core.h"
#include "SubSystemManager.h"
#include "Window/WindowSystem.h"

namespace RyEngine
{
	WindowSystem SubSystemManager::windowSystem()
	{
		return *_windowSystem;
	}

	GraphicsSystem SubSystemManager::graphicsSystem()
	{
		return *_graphicsSystem;
	}


	void SubSystemManager::RegisterSubSystems()
	{
		AddAllSubSystems();
		
		for (size_t i = 0; i < _allSubSystems.size(); i++)
		{
			Register(_allSubSystems[i]);
		}
	}

	void SubSystemManager::AddAllSubSystems()
	{
		_windowSystem = new WindowSystem();
		Add(_windowSystem);

		_graphicsSystem = new GraphicsSystem();
		Add(_graphicsSystem);
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

	void SubSystemManager::Register(SubSystem* s)
	{
		s->OnRegister();
	}
}
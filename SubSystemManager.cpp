#include "stdafx.h"
#include "SubSystemManager.h"

namespace RyEngine
{
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
		AddRenderable(_graphicsSystem);
	}

	void SubSystemManager::Release()
	{
		for (size_t i = 0; i < _allSubSystems.size(); i++)
		{
			_allSubSystems[i]->Release();
		}

		_updatableSubSystems.clear();
		_allSubSystems.clear();
		_renderableSubSystems.clear();

		delete _windowSystem;
		delete _graphicsSystem;
	}

	void SubSystemManager::SpinUpSubSystems()
	{
		for (size_t i = 0; i < _allSubSystems.size(); i++)
		{
			_allSubSystems[i]->OnStart();
		}
	}

	void SubSystemManager::OnUpdate()
	{

	}

	void SubSystemManager::OnRender()
	{
		for (size_t i = 0; i < _renderableSubSystems.size(); i++)
		{
			_renderableSubSystems[i]->OnRender();
		}
	}

	void SubSystemManager::AddUpdatable(UpdatableSubSystem* s)
	{
		_updatableSubSystems.push_back(s);
		Add(s);
	}

	void SubSystemManager::AddRenderable(RenderableSubSystem* s)
	{
		_renderableSubSystems.push_back(s);
		Add(s);
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
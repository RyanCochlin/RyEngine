#include "pch.h"
#include "SubSystemManager.h"

namespace RE
{
	void SubSystemManager::RegisterSubSystems()
	{
		AddAllSubSystems();
		
		std::vector<SubSystem*>::iterator it = _mAllSubSystems.begin();
		for (;it != _mAllSubSystems.end(); it++)
		{
			(*it)->OnRegister();
		}
	}

	void SubSystemManager::AddAllSubSystems()
	{
		_mAllSubSystems.push_back(new Log());
		_mAllSubSystems.push_back(new WindowSystem());
		_mAllSubSystems.push_back(new GraphicsSystem());
		_mAllSubSystems.push_back(new CameraManager());
	}

	void SubSystemManager::Release()
	{
		for (size_t i = 0; i < _mAllSubSystems.size(); i++)
		{
			_mAllSubSystems[i]->Release();
		}

		_mAllSubSystems.clear();
	}

	void SubSystemManager::SpinUpSubSystems()
	{
		for (size_t i = 0; i < _mAllSubSystems.size(); i++)
		{
			_mAllSubSystems[i]->OnStart();
		}
	}

	void SubSystemManager::OnUpdate()
	{
		for (size_t i = 0; i < _mAllSubSystems.size(); i++)
		{
			_mAllSubSystems[i]->OnUpdate();
		}
	}

	void SubSystemManager::OnRender()
	{
		for (size_t i = 0; i < _mAllSubSystems.size(); i++)
		{
			_mAllSubSystems[i]->OnRender();
		}
	}
}
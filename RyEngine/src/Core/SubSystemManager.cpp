#include "pch.h"
#include "SubSystemManager.h"

namespace RE
{
	void SubSystemManager::RegisterSubSystems()
	{
		AddAllSubSystems();
		
		for (auto subSystem : _mAllSubSystems)
		{
			subSystem->OnRegister();
		}
	}

	void SubSystemManager::AddAllSubSystems()
	{
		_mAllSubSystems.push_back(new Log());
		_mAllSubSystems.push_back(new WindowSystem());
		_mAllSubSystems.push_back(new InputSystem());
		_mAllSubSystems.push_back(new GraphicsSystem());
		_mAllSubSystems.push_back(new CameraManager());
		_mAllSubSystems.push_back(new FileSystem());
		//_mAllSubSystems.push_back(new EcsSystem());
	}

	void SubSystemManager::Release()
	{
		for (auto subSystem : _mAllSubSystems)
		{
			subSystem->Release();
		}

		_mAllSubSystems.clear();
	}

	void SubSystemManager::SpinUpSubSystems()
	{
		for (auto subSystem : _mAllSubSystems)
		{
			subSystem->OnStart();
		}
	}

	void SubSystemManager::OnUpdate()
	{
		for (auto subSystem : _mAllSubSystems)
		{
			subSystem->OnUpdate();
		}
	}

	void SubSystemManager::OnRender()
	{
		for (auto subSystem : _mAllSubSystems)
		{
			subSystem->OnRender();
		}
	}
}
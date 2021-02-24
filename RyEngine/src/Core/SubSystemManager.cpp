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
			Register(*it);
		}
	}

	void SubSystemManager::AddAllSubSystems()
	{
		_mLog = new Log();
		Add(_mLog);

		_mWindowSystem = new WindowSystem();
		Add(_mWindowSystem);

		_mGraphicsSystem = new GraphicsSystem();
		Add(_mGraphicsSystem);

		_mCameraManager = new CameraManager();
		Add(_mCameraManager);
	}

	void SubSystemManager::Release()
	{
		for (size_t i = 0; i < _mAllSubSystems.size(); i++)
		{
			_mAllSubSystems[i]->Release();
		}

		_mAllSubSystems.clear();

		//delete mLog;
		delete _mWindowSystem;
		delete _mGraphicsSystem;
		delete _mCameraManager;
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

	void SubSystemManager::Add(SubSystem* s)
	{
		_mAllSubSystems.push_back(s);
	}

	void SubSystemManager::Register(SubSystem* s)
	{
		s->OnRegister();
	}
}
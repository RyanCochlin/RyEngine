#include "pch.h"
#include "SubSystemManager.h"

namespace RE
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
		mLog = new Log();
		Add(mLog);

		mWindowSystem = new WindowSystem();
		Add(mWindowSystem);

		mGraphicsSystem = new GraphicsSystem();
		Add(mGraphicsSystem);
	}

	void SubSystemManager::Release()
	{
		for (size_t i = 0; i < _allSubSystems.size(); i++)
		{
			_allSubSystems[i]->Release();
		}

		_allSubSystems.clear();

		//delete mLog;
		delete mWindowSystem;
		delete mGraphicsSystem;
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
		for (size_t i = 0; i < _allSubSystems.size(); i++)
		{
			_allSubSystems[i]->OnUpdate();
		}
	}

	void SubSystemManager::OnRender()
	{
		for (size_t i = 0; i < _allSubSystems.size(); i++)
		{
			_allSubSystems[i]->OnRender();
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
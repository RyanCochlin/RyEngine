#pragma once

#include "Singleton.h"
#include "SubSystem.h"
#include "WindowSystem.h"
#include "GraphicsSystem.h"

#include <vector>

namespace RE
{
	class SubSystemManager : public Singleton<SubSystemManager>
	{
	public:
		//Getter for every subsystem. Maybe figure out a better way to do this
		Log* Logger()
		{
			return mLog;
		}

		WindowSystem* Wynd()
		{
			return mWindowSystem;
		}

		GraphicsSystem* RyFX()
		{
			return mGraphicsSystem;
		}

		void SpinUpSubSystems();
		void RegisterSubSystems();
		void Release();
		
		void OnUpdate();
		void OnRender();

	private:
		std::vector<SubSystem*> _allSubSystems;

		//Refs to subsystems
		WindowSystem* mWindowSystem;
		GraphicsSystem* mGraphicsSystem;
		Log* mLog;
		
		void Register(SubSystem* s);
		void Add(SubSystem* s);
		void AddAllSubSystems();
	};
}

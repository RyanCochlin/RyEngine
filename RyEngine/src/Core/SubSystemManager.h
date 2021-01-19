#pragma once

#include "Singleton.h"
#include "SubSystem.h"
#include "Window/WindowSystem.h"
#include "GraphicsSystem.h"

#include <vector>

namespace RE
{
	class RE_API SubSystemManager : public Singleton<SubSystemManager>
	{
	public:
		//Getter for every subsystem. Maybe figure out a better way to do this
		Log* Logger()
		{
			return _mLog;
		}

		WindowSystem* Wnd()
		{
			return _mWindowSystem;
		}

		GraphicsSystem* GFX()
		{
			return _mGraphicsSystem;
		}

		void SpinUpSubSystems();
		void RegisterSubSystems();
		void Release();
		
		void OnUpdate();
		void OnRender();

	private:
		std::vector<SubSystem*> _mAllSubSystems;

		//Refs to subsystems
		WindowSystem* _mWindowSystem;
		GraphicsSystem* _mGraphicsSystem;
		Log* _mLog;
		
		void Register(SubSystem* s);
		void Add(SubSystem* s);
		void AddAllSubSystems();
	};
}

#define RE_GRAPHICS ::RE::SubSystemManager::Instance().GFX()
#define RE_WINDOW ::RE::SubSystemManager::Instance().Wnd()
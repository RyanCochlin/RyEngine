#pragma once

#include "Utils/Singleton.h"
#include "SubSystem.h"
#include "Platform/Window/WindowSystem.h"

#include <vector>

namespace RyEngine
{
	class SubSystemManager : public Singleton<SubSystemManager>
	{
	public:
		//Getter for every subsystem. Maybe figure out a better way to do this
		WindowSystem windowSystem();

		void SpinUpSubSystems();
		void RegisterSubSystems();

	private:
		std::vector<SubSystem*> _allSubSystems;

		//Refs to subsystems
		WindowSystem* _windowSystem;
		
		void Register(SubSystem* s);
		void Add(SubSystem* s);
		void AddAllSubSystems();
	};
}

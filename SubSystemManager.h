#pragma once

#include "Singleton.h"
#include "SubSystem.h"
#include "UpdatableSubSystem.h"
#include "RenderableSubSystem.h"
#include "WindowSystem.h"
#include "GraphicsSystem.h"

#include <vector>

namespace RyEngine
{
	class SubSystemManager : public Singleton<SubSystemManager>
	{
	public:
		//Getter for every subsystem. Maybe figure out a better way to do this
		WindowSystem windowSystem()
		{
			return *_windowSystem;
		}

		GraphicsSystem graphicsSystem()
		{
			return *_graphicsSystem;
		}

		void SpinUpSubSystems();
		void RegisterSubSystems();
		void Release();
		
		void OnUpdate();
		void OnRender();

	private:
		std::vector<SubSystem*> _allSubSystems;
		std::vector<UpdatableSubSystem*> _updatableSubSystems;
		std::vector<RenderableSubSystem*> _renderableSubSystems;

		//Refs to subsystems
		WindowSystem* _windowSystem;
		GraphicsSystem* _graphicsSystem;
		
		void Register(SubSystem* s);
		void Add(SubSystem* s);
		void AddUpdatable(UpdatableSubSystem* s);
		void AddRenderable(RenderableSubSystem* s);
		void AddAllSubSystems();
	};
}

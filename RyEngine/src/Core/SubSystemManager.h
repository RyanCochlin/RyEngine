#pragma once

#include "Singleton.h"
#include "SubSystem.h"
#include "Window/WindowSystem.h"
#include "Input/InputSystem.h"
#include "GraphicsSystem.h"
#include "CameraManager.h"

#include <vector>

namespace RE
{
	class RE_API SubSystemManager : public Singleton<SubSystemManager>
	{
	public:
		template<typename T>
		T* GetSubSystem()
		{
			SubSystemType type = T::GetStaticType();
			for (auto subSystem : _mAllSubSystems)
			{
				if (subSystem->GetSubSystemType() == type)
				{
					return dynamic_cast<T*>(subSystem);
				}
			}

			return nullptr;
		}

		/////////////////////////////////////////////////////////////////////////////

		void SpinUpSubSystems();
		void RegisterSubSystems();
		void Release();
		
		void OnUpdate();
		void OnRender();

	private:
		std::vector<SubSystem*> _mAllSubSystems;
		
		void AddAllSubSystems();
	};
}

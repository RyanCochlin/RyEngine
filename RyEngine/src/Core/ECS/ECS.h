#pragma once

#include "Core/SubSystem.h"

namespace RE
{
	class Entity
	{
	public:
		Transform* transform;
		Mesh* mesh;
	};

	class EcsSystem : public SubSystem
	{
		RE_DECLARE_SUBSYSTEM(EcsSystem)
		
		virtual void OnRegister() override {};
		virtual void OnStart() override {};
		virtual void OnUpdate() override {};
		virtual void OnRender() override {};
		virtual void Release() override {};
	};
}
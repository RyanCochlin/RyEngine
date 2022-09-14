#pragma once

#include "Core/SubSystem.h"
#include "Core/Graphics/Material.h"

namespace RE
{
	class Entity
	{
	public:
		Transform* transform;
		Mesh* mesh;
		const Material* material;
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
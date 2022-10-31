#pragma once

#include "Core/GlobalDefines.h"
#include "Core/Graphics/Light.h"
#include "Core/SubSystemManager.h"

namespace RE::LightSystem
{
	RE_API Light* GetLight(LightType type);

	Light* GetLight(LightType type)
	{
		Light* light = new Light();
		light->Type = type;
		SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->AddLight(light);
		return light;
	}
}
#pragma once

#include "IGraphicsAPI.h"
#include "SubSystems/SubSystem.h"

#ifndef __ry_graphics_api__
#define __ry_graphics_api__

typedef enum RYGRAPHICS_API
{
	RYGRAPHICS_API_DIRECTX = 0
} RYGRAPHICS_API;

#endif // !__ry_graphics_api__

namespace RyEngine
{
	class GraphicsSystem : public SubSystem
	{
	public:
		void OnRegister() override;
		void OnStart() override;

	private:
		RYGRAPHICS_API _apiType = RYGRAPHICS_API_DIRECTX;
		IGraphicsAPI* _graphicsAPI;

	};
}
#pragma once

#include "IGraphicsAPI.h"
#include "RenderableSubSystem.h"

namespace RyEngine
{
	class GraphicsSystem : public RenderableSubSystem
	{
	public:
		void OnRegister() override;
		void OnStart() override;
		void OnRender() override;
		void Release() override;

		UINT GraphicsAPI();

	private:
		UINT _apiType = RYGRAPHICS_API_DIRECTX;
		IGraphicsAPI* _graphicsAPI;
	};
}
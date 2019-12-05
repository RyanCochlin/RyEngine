#include "GraphicsSystem.h"
#include "DirectX/DirectX.h"

namespace RyEngine
{
	void RyEngine::GraphicsSystem::OnRegister()
	{
		switch (_apiType)
		{
		case RYGRAPHICS_API_DIRECTX:
			_graphicsAPI = new DirectX();
		default:
			//Default is DirectX because that's the only graphics API currently supported
			_graphicsAPI = new DirectX();
		}

		_graphicsAPI->Init();
	}

	void GraphicsSystem::OnStart()
	{

	}
}

#include "stdafx.h"
#include "GraphicsSystem.h"
#include "RyDirectX.h"

namespace RyEngine
{
	void GraphicsSystem::OnRegister()
	{
		switch (_apiType)
		{
		case RYGRAPHICS_API_DIRECTX:
			_graphicsAPI = new RyDirectX();
			break;
		default:
			//Default is DirectX because that's the only graphics API currently supported
			_graphicsAPI = new RyDirectX();
			break;
		}

		_graphicsAPI->Init();
	}

	void GraphicsSystem::OnStart()
	{

	}

	void GraphicsSystem::Release()
	{
		_graphicsAPI->Release();
		delete _graphicsAPI;
	}

	void GraphicsSystem::OnRender()
	{
		_graphicsAPI->OnRender();
	}

	UINT GraphicsSystem::GraphicsAPI()
	{
		return _apiType;
	}
}

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#ifdef _DEBUG
//	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//	#define new DEBUG_NEW
//#endif

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
			break;
		default:
			//Default is DirectX because that's the only graphics API currently supported
			_graphicsAPI = new DirectX();
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
}

#include "pch.h"
#include "GraphicsSystem.h"
#include "Platform/DirectX/RyDirectX.h"

namespace RE
{
	void GraphicsSystem::OnRegister()
	{
		switch (CURRENT_GRAPHICS_API)
		{
		case GFX_API_DIRECTX:
		default:
			//Default is DirectX because that's the only graphics API currently supported
			_mGraphicsAPI = new RyDirectX();
			break;
		}

		_mGraphicsAPI->Init();
	}

	void GraphicsSystem::OnStart()
	{

	}

	void GraphicsSystem::Release()
	{
		_mGraphicsAPI->Release();
		delete _mGraphicsAPI;
	}

	void GraphicsSystem::OnUpdate()
	{

	}

	void GraphicsSystem::OnRender()
	{
		_mGraphicsAPI->OnRender();
	}

#ifdef RE_WINDOWS
	RyDirectX* GraphicsSystem::GraphicsAPI()
	{
		return static_cast<RyDirectX*>(_mGraphicsAPI);
	}
#else
	IGraphicsAPI* GraphicsSystem::GraphicsAPI()
	{
		return _mGraphicsAPI;
	}
#endif
}

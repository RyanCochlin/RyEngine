#include "pch.h"
#include "GraphicsSystem.h"
#include "RyDirectX.h"

namespace RE
{
	void GraphicsSystem::OnRegister()
	{
		switch (RYFX_API)
		{
		case RYFX_DIRECTX:
			mGraphicsAPI = new RyDirectX();
			break;
		default:
			//Default is DirectX because that's the only graphics API currently supported
			mGraphicsAPI = new RyDirectX();
			break;
		}

		mGraphicsAPI->Init();
	}

	void GraphicsSystem::OnStart()
	{

	}

	void GraphicsSystem::Release()
	{
		mGraphicsAPI->Release();
		delete mGraphicsAPI;
	}

	void GraphicsSystem::OnUpdate()
	{

	}

	void GraphicsSystem::OnRender()
	{
		mGraphicsAPI->OnRender();
	}

	IGraphicsAPI* GraphicsSystem::GraphicsAPI()
	{
		return mGraphicsAPI;
	}
}

#include "pch.h"
#include "GraphicsSystem.h"
#include "Platform/DirectX/DirectxAPI.h"

namespace RE
{
	void GraphicsSystem::OnRegister()
	{
		switch (CURRENT_GRAPHICS_API)
		{
		case GFX_API_DIRECTX:
		default:
			//Default is DirectX because that's the only graphics API currently supported
			_mGraphicsAPI = new DirectxAPI();
			break;
		}

		_mNewMesh = false;
		_mGeo = new GeometryHeap();
	}

	void GraphicsSystem::OnStart()
	{
		_mGraphicsAPI->Init();
	}

	void GraphicsSystem::Release()
	{
		_mGraphicsAPI->Release();
		delete _mGraphicsAPI;
	}

	void GraphicsSystem::OnUpdate()
	{
		if (_mNewMesh)
		{
			_mGraphicsAPI->SetGeometry(_mGeo);
			_mNewMesh = false;
		}
		_mGraphicsAPI->OnUpdate();
	}

	void GraphicsSystem::OnRender()
	{
		_mGraphicsAPI->OnRender();
	}

	IGraphicsAPI* GraphicsSystem::GraphicsAPI()
	{
		return _mGraphicsAPI;
	}

	void GraphicsSystem::AddMeshForDraw(Mesh* mesh)
	{
		//TODO want to only set geometery once per frame. Need to figure out how to manage them
		_mGeo->PushMesh(mesh);
		_mNewMesh = true;
	}

	void GraphicsSystem::RemoveMesh(Mesh* mesh)
	{
		_mGeo->RemoveMesh(mesh);
	}

	void GraphicsSystem::BackgroundColor(Color color)
	{
		_mGraphicsAPI->SetClearColor(color);
	}
}

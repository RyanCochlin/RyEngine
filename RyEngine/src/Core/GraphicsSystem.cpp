#include "pch.h"
#include "GraphicsSystem.h"
#include "SubSystemManager.h"
#include "Camera.h"
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

		delete _mGeo;
	}

	void GraphicsSystem::OnUpdate()
	{
		if (_mNewMesh)
		{
			_mGraphicsAPI->SetGeometry(_mGeo);
			_mNewMesh = false;
		}

		//TODO for now 1 draw call per camera. Probably want to reivew this later
		std::vector<Camera*> cams = SubSystemManager::Instance().GetSubSystem<CameraManager>()->GetCameras();
		for (std::vector<Camera*>::iterator i = cams.begin(); i != cams.end(); i++)
		{
			Camera* cam = *i;
			DrawCall dc;
			dc.SetMVP(cam->GetModelView());
			_mGraphicsAPI->PushDrawCall(dc);
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

#include "pch.h"

#include <memory>

#include "GraphicsSystem.h"
#include "MeshManager.h"
#include "Core/SubSystemManager.h"
#include "Core/Camera.h"
#include "Platform/DirectX/DirectxAPI.h"

namespace RE
{
	GraphicsSystem::GraphicsSystem() :
		_mGraphicsAPI()
		, _mMeshManager()
		, _mMeshHeap()
	{}

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

		_mMeshHeap = new MeshHeap();
		_mMeshManager = std::make_unique<MeshManager>();
	}

	void GraphicsSystem::OnStart()
	{
		_mGraphicsAPI->Init();
	}

	void GraphicsSystem::Release()
	{
		_mGraphicsAPI->Release();
		delete _mGraphicsAPI;
		delete _mMeshHeap;
	}

	void GraphicsSystem::OnUpdate()
	{
		//TODO update what I'm calling draw calls. This isn't right
		//TODO for now 1 draw call per camera. Probably want to reivew this later
		std::vector<Camera*> cams = SubSystemManager::Instance().GetSubSystem<CameraManager>()->GetCameras();
		for (auto& cam : cams)
		{
			DrawCall dc;
			dc.SetMVP(cam->GetModelView());
			_mGraphicsAPI->PushDrawCall(dc);
		}

		_mMeshManager->OnUpdate();

		MeshHeapData instanceData;
		if (_mMeshManager->FlushMeshData(instanceData))
		{
			_mGraphicsAPI->PushMeshData(instanceData);
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

	void GraphicsSystem::AddMesh(Mesh* mesh, Transform* trans)
	{
		_mMeshManager->AddMesh(mesh, trans);
	}

	void GraphicsSystem::BackgroundColor(Color color)
	{
		_mGraphicsAPI->SetClearColor(color);
	}
}

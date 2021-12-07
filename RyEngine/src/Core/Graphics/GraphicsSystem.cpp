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
		, _mNewMesh(false)
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

		_mNewMesh = false;
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
		if (_mNewMesh)
		{
			_mGraphicsAPI->SetGeometry(_mMeshHeap);
			_mNewMesh = false;
		}

		//TODO update what I'm calling draw calls. This isn't right
		//TODO for now 1 draw call per camera. Probably want to reivew this later
		std::vector<Camera*> cams = SubSystemManager::Instance().GetSubSystem<CameraManager>()->GetCameras();
		for (std::vector<Camera*>::iterator i = cams.begin(); i != cams.end(); i++)
		{
			Camera* cam = *i;
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

	void GraphicsSystem::AddMeshForDraw(Mesh& mesh)
	{
		//TODO want to only set geometery once per frame. Need to figure out how to manage them
		_mMeshHeap->PushMesh(mesh);
		_mNewMesh = true;
	}

	void GraphicsSystem::AddMesh(Mesh& mesh, Transform& trans)
	{
		_mMeshManager->AddMesh(mesh, trans);
	}

	void GraphicsSystem::BackgroundColor(Color color)
	{
		_mGraphicsAPI->SetClearColor(color);
	}
}

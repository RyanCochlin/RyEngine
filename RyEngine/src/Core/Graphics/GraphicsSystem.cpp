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
		_mMaterialManager = std::make_unique<MaterialManager>();
		_mLightManager = std::make_unique<LightManager>();
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
			dc.SetAmbient(_mLightManager->GetAmbientLight());

			for (auto light : _mLightManager->GetLights())
			{
				dc.AddLight(light);
			}

			//TODO figure out eye position. Right now it's at origin
			dc.SetEyePosition(cam->GetPosition());
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

	void GraphicsSystem::AddMesh(Mesh* mesh, Transform* trans, Material* mat)
	{
		_mMeshManager->AddMesh(mesh, trans, mat);
	}

	Mesh* GraphicsSystem::GetMeshInstance(const char* id)
	{
		return _mMeshManager->GetInstance(id);
	}

	Material* GraphicsSystem::GetDefaultMaterial(const char* name)
	{
		return _mMaterialManager->GetDefaultMaterial(name);
	}

	void GraphicsSystem::AddMeshInstance(const char* id, Transform* trans, Material* mat)
	{
		_mMeshManager->AddInstance(id, trans, mat);
	}

	void GraphicsSystem::BackgroundColor(Color color)
	{
		_mGraphicsAPI->SetClearColor(color);
	}

	void GraphicsSystem::SetAmbientLight(Color color)
	{
		_mLightManager->SetAmbientLight(ColorToVector(color));
	}

	Color GraphicsSystem::GetAmbientLight()
	{
		Vector4 light = _mLightManager->GetAmbientLight();
		Color lightColor;
		lightColor.c.r = light.x;
		lightColor.c.g = light.y;
		lightColor.c.b = light.z;
		lightColor.c.a = light.w;
		return lightColor;
	}

	void GraphicsSystem::AddLight(Light* light)
	{
		_mLightManager->AddLight(light);
	}
}

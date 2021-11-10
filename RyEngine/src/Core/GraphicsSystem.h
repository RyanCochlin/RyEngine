#pragma once

#include "pch.h"
#include "SubSystem.h"
#include "IGraphicsAPI.h"
#include "Mesh.h"
#include "Color.h"
#include "GeometryHeap.h"
#include "Graphics/DrawCall.h"
#include "Platform/DirectX/DirectXCore.h"

#ifndef __ry_graphics_api__
#define __ry_graphics_api__
#define RYGRAPHICS_API_DIRECTX 1
#define CURRENT_GRAPHICS_API RYGRAPHICS_API_DIRECTX;
#endif // __ry_graphics_api__


namespace RE
{
	enum GFX_API_TYPE
	{
		GFX_API_NONE = 0x00,
		GFX_API_DIRECTX = 0x01,
		GFX_API_OPENGL = 0x02
	};

#ifdef RE_WINDOWS
#define CURRENT_GRAPHICS_API GFX_API_DIRECTX
#else
#define CURRENT_GRAPHICS_API GFX_API_NONE
#endif

	class RE_API GraphicsSystem : public SubSystem
	{
	public:
		RE_DECLARE_SUBSYSTEM(GraphicsSystem)

		IGraphicsAPI* GraphicsAPI();
		void AddMeshForDraw(Mesh* mesh);
		void RemoveMesh(Mesh* mesh);
		void BackgroundColor(Color color);

		void OnRegister() override;
		void OnStart() override;
		void OnUpdate() override;
		void OnRender() override;
		void Release() override;

	private:
		IGraphicsAPI* _mGraphicsAPI;
		GeometryHeap* _mGeo;
		bool _mNewMesh;
	};
}

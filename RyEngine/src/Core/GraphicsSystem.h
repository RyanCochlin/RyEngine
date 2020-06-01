#pragma once

#include "pch.h"
#include "SubSystem.h"
#include "IGraphicsAPI.h"
#include "Platform/DirectX/RyDirectX.h"

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

	class GraphicsSystem : public SubSystem
	{
	public:
		RE_SUBSYSTEM_OVERRIDE

#ifdef RE_WINDOWS
		RyDirectX* GraphicsAPI();
#else
		IGraphicsAPI* GraphicsAPI();
#endif

	private:
		IGraphicsAPI* _mGraphicsAPI;
	};	
}

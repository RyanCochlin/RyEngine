#pragma once

#include "pch.h"
#include "SubSystem.h"
#include "IGraphicsAPI.h"

#ifndef __ry_graphics_api__
#define __ry_graphics_api__
#define RYGRAPHICS_API_DIRECTX 1
#define CURRENT_GRAPHICS_API RYGRAPHICS_API_DIRECTX;
#endif // __ry_graphics_api__


namespace RE
{
	#define RYFX_API_DX 0
	#define RYFX_API_OPENGL 1

	#define RYFX_API RYFX_API_DX

	enum RYFX_API_TYPE
	{
		RYFX_DIRECTX = RYFX_API_DX,
		RYFX_OPENGL = RYFX_API_OPENGL
	};

	class GraphicsSystem : public SubSystem
	{
	public:
		RE_SUBSYSTEM_OVERRIDE

		IGraphicsAPI* GraphicsAPI();

	private:
		IGraphicsAPI* mGraphicsAPI;
	};	
}

#pragma once

#include "stdafx.h"

#include "SubSystemManager.h"
#include "GraphicsSystem.h"

namespace RyEngine
{
	#if CURRENT_GRAPHICS_API==RYGRAPHICS_API_DIRECTX

	#define Vector3 DirectX::XMFLOAT3
	#define Vector4 DirectX::XMFLOAT4

	#else

	typedef struct Vector3
	{
		float x;
		float y;
		float z;
	} Vector3;

	typedef struct Vector4
	{
		float x;
		float y;
		float z;
		float w;
	} Vector4;

	#endif
}

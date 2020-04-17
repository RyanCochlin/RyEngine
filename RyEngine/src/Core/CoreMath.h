#pragma once

#include "pch.h"

#include "SubSystemManager.h"
#include "GraphicsSystem.h"

namespace RE
{
	#if RYFX_API == RYFX_API_DX

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

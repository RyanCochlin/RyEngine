#pragma once
#include "pch.h"

namespace RE
{
	class DXVertex
	{
	public:
		DXVertex() :_mVert(0, 0, 0) {}
		DXVertex(float x, float y, float z) : _mVert(x, y, z) {}

		float X() { return _mVert.x; }
		float Y() { return _mVert.y; }
		float Z() { return _mVert.z; }

		//TODO math stuff

		DirectX::XMFLOAT3 _mVert;
	};
}
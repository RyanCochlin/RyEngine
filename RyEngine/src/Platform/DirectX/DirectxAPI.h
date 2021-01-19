#pragma once

#include "DirectXCore.h"
#include "Core/Color.h"
#include "Core/IGraphicsAPI.h"
#include "Core/GeometryHeap.h"

namespace RE
{
	class DirectxAPI : public IGraphicsAPI
	{
	public:
		DirectxAPI() {}

		void Init() override { _mDirectX.Init(); }
		void Release() override { _mDirectX.Release(); }
		void OnUpdate() override { _mDirectX.Update(); }
		void OnRender() override { _mDirectX.OnRender(); }


		void SetClearColor(Color color) override { _mDirectX.SetClearColor(color); }
		void SetGeometry(GeometryHeap* geo) override { _mDirectX.SubmitGeometery(geo); }

	private:
		DirectXCore _mDirectX;
		GeometryHeap* _mGeo;
	};
}
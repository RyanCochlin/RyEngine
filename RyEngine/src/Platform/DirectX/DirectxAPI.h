#pragma once

#include "DirectXCore.h"
#include "DXDrawCall.h"
#include "Core/Color.h"
#include "Core/Graphics/IGraphicsAPI.h"
#include "Core/Graphics/MeshHeap.h"
#include "Core/Graphics/DrawCall.h"


namespace RE
{
	class DirectxAPI : public IGraphicsAPI
	{
	public:
		DirectxAPI() {}

		void Init() override { _mDirectX.Init(); }
		void Release() override { _mDirectX.Release(); }
		void OnRender() override { _mDirectX.OnRender(); }

		void OnUpdate() override
		{
			std::vector<DXDrawCall>::iterator i = _mDrawCalls.begin();
			for (; i != _mDrawCalls.end(); i++)
			{
				_mDirectX.PushDrawCall(*i);
			}
			_mDrawCalls.clear();

			_mDirectX.Update();
		}

		void PushMeshData(const MeshHeapData& meshHeap) override 
		{
			_mDirectX.SubmitMeshHeap(meshHeap);
		}

		void SetClearColor(Color color) override { _mDirectX.SetClearColor(color); }
		
		void PushDrawCall(DrawCall dc) override
		{
			DXDrawCall draw = dc;
			_mDrawCalls.push_back(draw);
		}

	private:
		DirectXCore _mDirectX;
		MeshHeap* _mGeo;
		std::vector<DXDrawCall> _mDrawCalls;
	};
}
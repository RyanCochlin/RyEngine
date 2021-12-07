#pragma once

#include "DrawCall.h"
#include "Core/ViewPort.h"
#include "Core/Color.h"
#include "MeshHeap.h"
#include "Core/Graphics/MeshManager.h"

namespace RE
{
	class IGraphicsAPI
	{
	public:
		virtual ~IGraphicsAPI() {}

		virtual void Init() = 0;
		virtual void Release() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;

		virtual void SetClearColor(Color color) = 0;
		virtual void SetGeometry(MeshHeap* geo) = 0;

		virtual void PushDrawCall(DrawCall dc) = 0;
		virtual void PushMeshData(const MeshHeapData& meshHeap) = 0;

	protected:
		ViewPort _mainViewPort;
	};
}

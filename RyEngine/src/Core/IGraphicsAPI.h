#pragma once

#include "Graphics/DrawCall.h"
#include "ViewPort.h"
#include "Color.h"
#include "GeometryHeap.h"

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
		virtual void SetGeometry(GeometryHeap* geo) = 0;

		virtual void PushDrawCall(DrawCall dc) = 0;

	protected:
		ViewPort _mainViewPort;
	};
}

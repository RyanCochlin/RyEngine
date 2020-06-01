#pragma once

#include "ViewPort.h"

namespace RE
{
	class IGraphicsAPI
	{
	public:
		virtual ~IGraphicsAPI() {}

		virtual void Init() = 0;
		virtual void Release() = 0;
		virtual void OnRender() = 0;

	protected:
		ViewPort _mainViewPort;
	};
}

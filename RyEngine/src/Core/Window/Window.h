#pragma once

#include <vector>

#include "Core/ViewPort.h"
#include "WindowSystem.h"

namespace RE
{
	class Window
	{
	public:
		Window() = default;
		virtual ~Window();
		virtual void Release() = 0;
		virtual bool ProcessEvents() = 0;
		WindowDimensions GetDimensions();
		FLOAT GetAspect();

	protected:
		UINT _mWidth;
		UINT _mHeight;
		std::vector<ViewPort> _mViewPorts;
	};
}


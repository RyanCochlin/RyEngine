#pragma once

#include <vector>

#include "Core/ViewPort.h"

namespace RE
{
	class Window
	{
	public:
		virtual ~Window();

		virtual void Release() = 0;

	protected:
		UINT _mWidth;
		UINT _mHeight;
		std::vector<ViewPort> _mViewPorts;
	};
}


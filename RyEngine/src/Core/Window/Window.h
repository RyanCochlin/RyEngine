#pragma once

#include <vector>

#include "Core/ViewPort.h"
#include "Core/Events/Event.h"
#include "WindowSystem.h"

namespace RE
{
	class Window
	{
	public:
		typedef void (*EventCallback)(Event&, void*);

		Window() = default;
		virtual ~Window();
		virtual void Release() = 0;
		virtual bool ProcessEvents() = 0;
		WindowDimensions GetDimensions();
		FLOAT GetAspect();
		void SetEventCallback(EventCallback callback, void* callee);

	protected:
		UINT _mWidth;
		UINT _mHeight;
		std::vector<ViewPort> _mViewPorts;
		std::unordered_map<void*, EventCallback> _mEventCallbacks;
	};
}


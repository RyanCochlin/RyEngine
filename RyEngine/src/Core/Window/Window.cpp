#include "pch.h"
#include "Window.h"

namespace RE
{
	Window::~Window() {}

	WindowDimensions Window::GetDimensions()
	{
		return WindowDimensions{ _mWidth, _mHeight }; 
	}

	FLOAT Window::GetAspect()
	{
		FLOAT aspect = (FLOAT)_mWidth / (FLOAT)_mHeight;
		return aspect;
	}

	void Window::SetEventCallback(EventCallback callback, void* callee)
	{
		_mEventCallbacks[callee] = callback;
	}
}
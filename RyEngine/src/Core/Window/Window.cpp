#include "pch.h"
#include "Window.h"

namespace RE
{
	Window::~Window() {}

	WindowDimensions Window::GetDimensions()
	{
		return WindowDimensions{ _mWidth, _mHeight }; 
	}

	float Window::GetAspect()
	{
		float aspect = (float)_mWidth / (float)_mHeight;
		return aspect;
	}

	void Window::SetEventCallback(EventCallback callback, void* callee)
	{
		_mEventCallbacks[callee] = callback;
	}
}
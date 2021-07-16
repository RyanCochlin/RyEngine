#pragma once

#include "Core/Window/Window.h"
#include "WindowClass.h"

namespace RE
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const wchar_t* winTitle, UINT w, UINT h, WIND_CLASS_TYPE wndClassType = WIND_CLASS_TYPE_MAIN, int cmdShow = SW_SHOWDEFAULT);

		virtual void Release() override;
		virtual bool ProcessEvents() override;
		HWND GetHandle() const { return _mWindHandle; }

	private:
		HWND _mWindHandle;

		static void OnWindowEvent(UINT message, WPARAM wParam, LPARAM lParam);
		static void OnInputEvent(UINT message, WPARAM wParam, LPARAM lParam);

		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}
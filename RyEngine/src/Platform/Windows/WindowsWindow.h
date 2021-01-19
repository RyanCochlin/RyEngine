#pragma once

#include "Core/Window/Window.h"
#include "WindowClass.h"

namespace RE
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const wchar_t* winTitle, UINT w, UINT h, WIND_CLASS_TYPE wndClass = WIND_CLASS_TYPE_MAIN, int cmdShow = SW_SHOWDEFAULT);

		void Release() override;

		HWND GetHandle() const { return _mWindHandle; }

	private:
		HWND _mWindHandle;

		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}
#pragma once

#include "Core/Window/Window.h"
#include "WindowClass.h"
#include "Core/Events/KeyboardEvents.h"

namespace RE
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const wchar_t* winTitle, UINT w, UINT h, WIND_CLASS_TYPE wndClassType = WIND_CLASS_TYPE_MAIN, int cmdShow = SW_SHOWDEFAULT);

		virtual void Release() override;
		virtual bool ProcessEvents() override;
		HWND GetHandle() const { return _mWindHandle; }

		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		HWND _mWindHandle;

		void OnWindowEvent(UINT message, WPARAM wParam, LPARAM lParam);
		void OnInputEvent(UINT message, WPARAM wParam, LPARAM lParam);
		void OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
		void ProcessEvent(Event& e);
	};
}
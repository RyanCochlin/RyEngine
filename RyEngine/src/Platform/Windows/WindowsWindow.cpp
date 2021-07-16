#include "pch.h"
#include "WindowsWindow.h"
#include "WindowClass.h"

namespace RE
{
	WindowsWindow::WindowsWindow(const wchar_t* winTitle, UINT w, UINT h, WIND_CLASS_TYPE wndClassType, int cmdShow)
	{
		_mWidth = w;
		_mHeight = h;

		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
		WindowClass* wndClass = WindowClassManager::RegisterWindowClass(wndClassType, hInstance, &OnWindowEvent, &OnInputEvent);
		const wchar_t* wndClassName = wndClass->GetName();

		RECT wr;
		wr.left = 0;
		wr.right = wr.left + _mWidth;
		wr.top = 0;
		wr.bottom = wr.top + _mHeight;
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

		_mWindHandle = CreateWindow(wndClassName,
			winTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			wr.right - wr.left,
			wr.bottom - wr.top,
			nullptr,
			nullptr,
			hInstance,
			wndClass);

		ShowWindow(_mWindHandle, cmdShow);
		UpdateWindow(_mWindHandle);
	}

	bool WindowsWindow::ProcessEvents()
	{
		MSG msg;
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return false;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return true;
	}

	void WindowsWindow::Release()
	{
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
		WindowClassManager::ReleaseWindowClass(WIND_CLASS_TYPE_MAIN, hInstance);
		DestroyWindow(_mWindHandle);
	}

	void WindowsWindow::OnWindowEvent(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}

	void WindowsWindow::OnInputEvent(UINT message, WPARAM wParam, LPARAM lParam)
	{
		//TODO handle input
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
}
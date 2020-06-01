#include "pch.h"
#include "WindowsWindow.h"
#include "WindowClass.h"

namespace RE
{
	WindowsWindow::WindowsWindow(const char* winTitle, UINT w, UINT h, WIND_CLASS_TYPE wndClass, int cmdShow)
	{
		_mWidth = w;
		_mHeight = h;

		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
		const char* wndClassName = WindowClassManager::RegisterWindowClass(wndClass, hInstance, WindowsWindow::WindowProc);
		const wchar_t* lClassName = AnsiToWString(wndClassName).c_str();

		_mWindHandle = CreateWindow(lClassName,
			AnsiToWString(winTitle).c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			_mWidth,
			_mHeight,
			nullptr,
			nullptr,
			hInstance,
			nullptr);

		ShowWindow(_mWindHandle, cmdShow);
		UpdateWindow(_mWindHandle);
	}

	void WindowsWindow::Release()
	{
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
		WindowClassManager::ReleaseWindowClass(WIND_CLASS_TYPE_MAIN, hInstance);
		DestroyWindow(_mWindHandle);
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_EXIT:
				PostQuitMessage(0);
				//DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
}
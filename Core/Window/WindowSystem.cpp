#include <Windows.h>

#include "Resource.h"
#include "WindowSystem.h"
#include "Window.h"

namespace RyEngine
{
	void WindowSystem::OnRegister()
	{
		ApplicationSubSystem::OnRegister();
		LoadStringW(_hInstance, IDS_APP_TITLE, _winTitle, STRING_BUFFER_MAX);
		LoadStringW(_hInstance, IDC_RYENGINE, _winClass, STRING_BUFFER_MAX);

		RegisterWindowClass();

		Window* w = AddWindow(_winTitle);
		_mainWin = w;
	}

	void WindowSystem::OnStart()
	{
		
	}

	Window* WindowSystem::get_mainWindow()
	{
		return _mainWin;
	}

	ATOM WindowSystem::RegisterWindowClass()
	{
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WindowSystem::WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = _hInstance;
		wcex.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_RYENGINE));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RYENGINE);
		wcex.lpszClassName = _winClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		return RegisterClassExW(&wcex);
	}

	LRESULT CALLBACK WindowSystem::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	Window* WindowSystem::AddWindow(LPWSTR winTitle)
	{
		return AddWindow(winTitle, SW_SHOWDEFAULT, _defaultHeight, _defaultWidth);
	}

	Window* WindowSystem::AddWindow(LPWSTR winTitle, int cmdShow, UINT width, UINT height)
	{
		Window* w = new Window(_hInstance, cmdShow, _winClass, winTitle, _defaultWidth, _defaultHeight);
		_windows.push_back(w);
		return w;
	}
}
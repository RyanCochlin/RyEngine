#include "pch.h"
#include "WindowSystem.h"

namespace RE
{
	void WindowSystem::OnRegister()
	{
		mhInstance = (HINSTANCE)GetModuleHandle(NULL);

		wchar_t* winClass = L"RyEngine";
		wcsncpy(mWinClass, winClass, sizeof(winClass));
		wcsncpy(mWinTitle, winClass, sizeof(winClass));
		//LoadStringW(mhInstance, IDS_APP_TITLE, mWinTitle, STRING_BUFFER_MAX);
		//LoadStringW(mhInstance, IDC_RYENGINE, mWinClass, STRING_BUFFER_MAX);

		RegisterWindowClass();

		Window* w = AddWindow(mWinTitle);
		mMainWin = w;
	}

	void WindowSystem::OnStart(){}
	void WindowSystem::OnRender() {}
	void WindowSystem::OnUpdate() {}

	void WindowSystem::Release()
	{
		for (size_t i = 0; i < mWindows.size(); i++)
		{
			delete mWindows[i];
		}

		mWindows.clear();

		UnregisterClassW(mWinClass, mhInstance);
	}

	Window* WindowSystem::GetMainWindow()
	{
		return mMainWin;
	}

	ATOM WindowSystem::RegisterWindowClass()
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WindowSystem::WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = mhInstance;
		wcex.hIcon = LoadIcon(mhInstance, MAKEINTRESOURCE(IDI_RYENGINE));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RYENGINE);
		wcex.lpszClassName = mWinClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		return RegisterClassExW(&wcex);
	}

	LRESULT CALLBACK WindowSystem::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

	Window* WindowSystem::AddWindow(LPWSTR winTitle)
	{
		return AddWindow(winTitle, SW_SHOWDEFAULT, mDefaultHeight, mDefaultWidth);
	}

	Window* WindowSystem::AddWindow(LPWSTR winTitle, int cmdShow, UINT width, UINT height)
	{
		Window* w = new Window(mhInstance, cmdShow, mWinClass, winTitle, mDefaultWidth, mDefaultHeight);
		mWindows.push_back(w);
		return w;
	}
}
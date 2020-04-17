#pragma once

#include <string>
#include <vector>

#include "SubSystem.h"
#include "Window.h"

namespace RE
{
	class WindowSystem : public SubSystem
	{
	public:
		RE_SUBSYSTEM_OVERRIDE
		
		Window* GetMainWindow();

		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		Window* mMainWin = NULL;
		std::vector<Window*> mWindows;
		HINSTANCE mhInstance = { };

		WCHAR mWinTitle[STRING_BUFFER_MAX];
		WCHAR mWinClass[STRING_BUFFER_MAX];
		UINT mDefaultWidth = 800;
		UINT mDefaultHeight = 600;

		ATOM RegisterWindowClass();
		Window* AddWindow(LPWSTR winTitle);
		Window* AddWindow(LPWSTR winTitle, int cmdShow, UINT width, UINT height);
	};
}

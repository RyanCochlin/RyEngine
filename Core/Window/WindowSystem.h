#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#ifdef _DEBUG
//	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//	#define new DEBUG_NEW
//#endif

#include <string>
#include <vector>

#include "SubSystems/ApplicationSubSystem.h"
#include "Window.h"

namespace RyEngine
{
	class WindowSystem : public ApplicationSubSystem
	{
	public:
		WindowSystem() : ApplicationSubSystem() {}

		void OnRegister();
		void OnStart();
		void Release();

		Window* get_mainWindow();

		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		Window* _mainWin = NULL;
		std::vector<Window*> _windows;

		WCHAR _winTitle[STRING_BUFFER_MAX];
		WCHAR _winClass[STRING_BUFFER_MAX];
		UINT _defaultWidth = 800;
		UINT _defaultHeight = 600;

		ATOM RegisterWindowClass();
		Window* AddWindow(LPWSTR winTitle);
		Window* AddWindow(LPWSTR winTitle, int cmdShow, UINT width, UINT height);
	};
}

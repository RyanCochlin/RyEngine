#pragma once

#include <string>
#include <vector>

#include "Core/SubSystem.h"
#include "Window.h"

namespace RE
{
#define WND_DEFAULT_WIDTH 1920
#define WND_DEFAULT_HEIGHT 1080
#define WND_DEFAULT_TITLE "RyEngine"

	class WindowSystem : public SubSystem
	{
	public:
		RE_SUBSYSTEM_OVERRIDE
		
		Window* GetMainWindow();

	private:
		Window* _mMainWin = NULL;
		std::vector<Window*> _mWindows;

		Window* AddWindow(const char* winTitle);
		Window* AddWindow(const char* winTitle, UINT width, UINT height);
	};
}

#pragma once

#include <string>
#include <vector>

#include "Core/SubSystem.h"

namespace RE
{
#define WND_DEFAULT_WIDTH 1920
#define WND_DEFAULT_HEIGHT 1080
#define WND_DEFAULT_TITLE L"RyEngine"

	class Window;

	struct WindowDimensions
	{
		UINT width;
		UINT height;
	};

	class WindowSystem : public SubSystem
	{
	public:
		RE_DECLARE_SUBSYSTEM(WindowSystem)

		Window* GetMainWindow();
		float GetMainWindowAspect();

		void OnRegister() override;
		void OnStart() override;
		void OnUpdate() override;
		void OnRender() override;
		void Release() override;

	private:
		Window* _mMainWin = nullptr;
		std::vector<Window*> _mWindows;

		Window* AddWindow(const wchar_t* winTitle);
		Window* AddWindow(const wchar_t* winTitle, UINT width, UINT height);
	};
}

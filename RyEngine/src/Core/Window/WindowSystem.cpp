#include "pch.h"
#include "WindowSystem.h"
#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace RE
{
	void WindowSystem::OnRegister()
	{
		Window* w = AddWindow(WND_DEFAULT_TITLE);
		_mMainWin = w;
	}

	void WindowSystem::OnStart(){}
	void WindowSystem::OnRender() {}
	void WindowSystem::OnUpdate() {}

	void WindowSystem::Release()
	{
		for (size_t i = 0; i < _mWindows.size(); i++)
		{
			_mWindows[i]->Release();
			delete _mWindows[i];
		}

		_mWindows.clear();
	}

	Window* WindowSystem::GetMainWindow()
	{
		return _mMainWin;
	}

	FLOAT WindowSystem::GetMainWindowAspect()
	{
		Window* window = GetMainWindow();
		FLOAT aspect = window->GetAspect();
		return window->GetAspect();
	}

	Window* WindowSystem::AddWindow(const wchar_t* winTitle)
	{
		return AddWindow(winTitle, WND_DEFAULT_WIDTH, WND_DEFAULT_HEIGHT);
	}

	Window* WindowSystem::AddWindow(const wchar_t* winTitle, UINT width, UINT height)
	{
		Window* w = nullptr;

#if RE_WINDOWS
		w = new WindowsWindow(winTitle, width, height);
#else
		RE_CORE_ERROR("No window implementation for current platform");
#endif

		if (w != nullptr)
		{
			_mWindows.push_back(w);
		}
		return w;
	}
}
#include "pch.h"
#include "Application.h"
#include "Core/SubSystemManager.h"
#include "Window/Window.h"

//TODO delete these includes
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Graphics/Transform.h"

namespace RE
{
	Application::Application() :
		_mMainWindow(0)
	{
	}

	Application::~Application()
	{
	}

	void Application::Start()
	{
		SubSystemManager::Instance().RegisterSubSystems();
		SubSystemManager::Instance().SpinUpSubSystems();

		_mMainWindow = SubSystemManager::Instance().GetSubSystem<WindowSystem>()->GetMainWindow();

		OnStart();
	}

	void Application::Run()
	{
		MSG msg = {};

		//TODO delete this. Just a test
		Transform t;
		t.Translate({ 1, 2, 3 });
		t.GetWorld();

		while (true)
		{
			if (!_mMainWindow->ProcessEvents())
			{
				break;
			}

			//1. Engine update loop
			SubSystemManager::Instance().OnUpdate();
			SubSystemManager::Instance().OnRender();

			//2. App update loop
			OnUpdate();
		}

		SubSystemManager::Instance().Release();

		//TODO return exit code
	}
}
#include "pch.h"
#include "Application.h"
#include "Core/SubSystemManager.h"

namespace RE
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Start()
	{
		RE::SubSystemManager::Instance().RegisterSubSystems();
		RE::SubSystemManager::Instance().SpinUpSubSystems();
	}

	void Application::Run()
	{
		MSG msg = {};

		while (msg.message != WM_QUIT)
		{
			//TODO: Handle input
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				SubSystemManager::Instance().OnUpdate();
				SubSystemManager::Instance().OnRender();
			}
		}
	}
}

// TODO: Reference from RyEngine.cpp
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_COMMAND:
//        {
//            int wmId = LOWORD(wParam);
//            // Parse the menu selections:
//            switch (wmId)
//            {
//            case IDM_ABOUT:
//                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//                break;
//            case IDM_EXIT:
//                DestroyWindow(hWnd);
//                break;
//            default:
//                return DefWindowProc(hWnd, message, wParam, lParam);
//            }
//        }
//        break;
//    case WM_PAINT:
//        {
//            PAINTSTRUCT ps;
//            HDC hdc = BeginPaint(hWnd, &ps);
//            // TODO: Add any drawing code that uses hdc here...
//            EndPaint(hWnd, &ps);
//        }
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
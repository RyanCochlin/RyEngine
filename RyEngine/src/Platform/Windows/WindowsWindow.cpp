#include "pch.h"
#include <memory>
#include "WindowsWindow.h"
#include "WindowClass.h"
#include "Core/Events/KeyboardEvents.h"
#include "Core/Events/MouseEvents.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/Keyboard.h"

namespace RE
{
	WindowsWindow::WindowsWindow(const wchar_t* winTitle, UINT w, UINT h, WIND_CLASS_TYPE wndClassType, int cmdShow)
	{
		_mWidth = w;
		_mHeight = h;

		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
		WindowClass* wndClass = WindowClassManager::RegisterWindowClass(wndClassType, hInstance);
		const wchar_t* wndClassName = wndClass->GetName();

		RECT wr;
		wr.left = 0;
		wr.right = wr.left + _mWidth;
		wr.top = 0;
		wr.bottom = wr.top + _mHeight;
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

		_mWindHandle = CreateWindow(wndClassName,
			winTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			wr.right - wr.left,
			wr.bottom - wr.top,
			nullptr,
			nullptr,
			hInstance,
			this);

		ShowWindow(_mWindHandle, cmdShow);
		UpdateWindow(_mWindHandle);
	}

	bool WindowsWindow::ProcessEvents()
	{
		MSG msg;
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return false;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return true;
	}

	void WindowsWindow::Release()
	{
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
		WindowClassManager::ReleaseWindowClass(WIND_CLASS_TYPE_MAIN, hInstance);
		DestroyWindow(_mWindHandle);
	}

	void WindowsWindow::OnWindowEvent(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}

	void WindowsWindow::OnInputEvent(UINT message, WPARAM wParam, LPARAM lParam)
	{
		//Don't need translate function because I stole the keycodes from windows keycodes
		RE_KEYCODE keyCode = static_cast<RE_KEYCODE>(wParam);

		switch (message)
		{
		case WM_KEYUP:
		{
			KeyUpEvent kue;
			kue.SetKeyCode(keyCode);
			ProcessEvent(kue);
		}
		break;
		case WM_KEYDOWN:
		{
			// This checks for repeat. If bit 30 of lParam is 1 this is a repeat press
			if (!(lParam & 0x40000000) || Keyboard::AutoRepeatEnabled())
			{
				KeyDownEvent kde;
				kde.SetKeyCode(keyCode);
				ProcessEvent(kde);
			}
		}
		break;
		}
	}

	void WindowsWindow::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
	{
		std::unique_ptr<MouseEvent> mouseEvent;
		
		switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			mouseEvent = std::make_unique<MouseDownEvent>();
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			mouseEvent = std::make_unique<MouseUpEvent>();
			//Bongo4 was here
			//float bongo = bongo1 + bongo2;
			break;
		case WM_MOUSEMOVE:
			mouseEvent = std::make_unique<MouseMoveEvent>();
			break;
		case WM_MOUSEWHEEL:
			mouseEvent = std::make_unique<MouseScrollEvent>();

			//divide by WHEEL_DELTA to get the number of ticks instead of size of deltas
			short delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			MouseScrollEvent* scrollEvent = dynamic_cast<MouseScrollEvent*>(mouseEvent.get());
			scrollEvent->SetWheelDelta(delta);
			break;
		}

		if (mouseEvent)
		{
			float x = (float)GET_X_LPARAM(lParam);
			float y = (float)GET_Y_LPARAM(lParam);
			mouseEvent->SetPosition({ x, y });
			ProcessEvent(*mouseEvent);
		}
	}

	void WindowsWindow::ProcessEvent(Event& e)
	{
		for (auto& i = _mEventCallbacks.begin(); i != _mEventCallbacks.end(); i++)
		{
			void* callee = (*i).first;
			EventCallback callback = (*i).second;
			callback(e, callee);
		}
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowsWindow* const wndClass = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return wndClass->HandleMessage(hWnd, message, wParam, lParam);
	}

	//TODO START HERE handle this in WindowsWindow instead of here 
	LRESULT WindowsWindow::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			OnWindowEvent(message, wParam, lParam);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			OnInputEvent(message, wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEMOVE:
			OnMouseEvent(message, wParam, lParam);
			break;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
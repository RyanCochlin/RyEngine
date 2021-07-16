#include "pch.h"
#include "WindowClass.h"

namespace RE
{
	WindowClass::WindowClass(const wchar_t* name, WindowEventHandler windowEvent, InputEventHandler inputEvent) :
		_mName(name),
		_mMenuName(""),
		_mWndRefCount(0),
		_mSize(0),
		_mStyle(0),
		_mIcon{},
		_mIconSm{},
		_mCursor{},
		_mBackground{},
		_mWindowEvent(windowEvent),
		_mInputEvent(inputEvent)
	{}

	void WindowClass::Release(HINSTANCE resHandle)
	{
		_mWndRefCount--;
		if (_mWndRefCount <= 0)
		{
			UnregisterClassW(_mName, resHandle);
		}
	}

	void WindowClass::InternalRegister(HINSTANCE resHandle)
	{
		WNDCLASSEX wcex;
		wcex.cbSize = _mSize;
		wcex.style = _mStyle;
		wcex.lpfnWndProc = WindowProcSetup;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = resHandle;
		wcex.hIcon = _mIcon;
		wcex.hCursor = _mCursor;
		wcex.hbrBackground = _mBackground;
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RYENGINE);
		wcex.lpszClassName = _mName;
		wcex.hIconSm = _mIconSm;

		RegisterClassExW(&wcex);
	}

	LRESULT CALLBACK WindowClass::WindowProcSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_NCCREATE)
		{
			const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowClass* const wndClass = static_cast<WindowClass*>(create->lpCreateParams);

			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wndClass));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowClass::WindowProc));

			return wndClass->HandleMessage(hWnd, message, wParam, lParam);
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT CALLBACK WindowClass::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowClass* const wndClass = reinterpret_cast<WindowClass*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return wndClass->HandleMessage(hWnd, message, wParam, lParam);
	}

	LRESULT WindowClass::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			(*_mWindowEvent)(message, wParam, lParam);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			(*_mInputEvent)(message, wParam, lParam);
			break;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	std::map<WIND_CLASS_TYPE, WindowClass*> WindowClassManager::_sRegisteredClasses{};

	WindowClass* WindowClassManager::RegisterWindowClass(WIND_CLASS_TYPE type, HINSTANCE resHandle, WindowEventHandler windowEvent, InputEventHandler inputEvent)
	{
		WindowClass* wndClass;
		if (_sRegisteredClasses.find(type) == _sRegisteredClasses.end())
		{
			wndClass = CreateClass(type, resHandle, windowEvent, inputEvent);
			ASSERT(wndClass != nullptr);

			_sRegisteredClasses[type] = wndClass;
		}
		else
		{
			wndClass = GetClass(type);
		}

		wndClass->Register();

		return _sRegisteredClasses[type];
	}

	void WindowClassManager::ReleaseWindowClass(WIND_CLASS_TYPE type, HINSTANCE resHandle)
	{
		WindowClass* wndClass = GetClass(type);
		if (wndClass != nullptr)
		{
			wndClass->Release(resHandle);
			if (!wndClass->IsRegistered())
			{
				delete wndClass;
				_sRegisteredClasses.erase(type);
			}
		}
	}

	WindowClass* WindowClassManager::CreateClass(WIND_CLASS_TYPE type, HINSTANCE resHandle, WindowEventHandler windowEvent, InputEventHandler inputEvent)
	{
		switch (type)
		{
		case RE::WIND_CLASS_TYPE_MAIN:
			return new WindowClassMain(resHandle, windowEvent, inputEvent);
		default:
			return nullptr;
		}
	}

	WindowClass* WindowClassManager::GetClass(WIND_CLASS_TYPE type)
	{
		std::map<WIND_CLASS_TYPE, WindowClass*>::iterator itt = _sRegisteredClasses.find(type);
		if (itt == _sRegisteredClasses.end())
		{
			return nullptr;
		}
		return (*itt).second;
	}
}
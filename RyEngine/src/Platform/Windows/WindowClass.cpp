#include "pch.h"
#include "WindowClass.h"

namespace RE
{
	WindowClass::WindowClass(const wchar_t* name) :
		_mName(name),
		_mMenuName(""),
		_mWndRefCount(0),
		_mSize(0),
		_mStyle(0),
		_mIcon{},
		_mIconSm{},
		_mCursor{},
		_mBackground{}
	{}

	void WindowClass::Release(HINSTANCE resHandle)
	{
		_mWndRefCount--;
		if (_mWndRefCount <= 0)
		{
			UnregisterClassW(_mName, resHandle);
		}
	}

	void WindowClass::InternalRegister(HINSTANCE resHandle, WNDPROC wndProc)
	{
		WNDCLASSEX wcex;
		wcex.cbSize = _mSize;
		wcex.style = _mStyle;
		wcex.lpfnWndProc = wndProc;
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

	std::map<WIND_CLASS_TYPE, WindowClass*> WindowClassManager::_sRegisteredClasses{};

	const wchar_t* WindowClassManager::RegisterWindowClass(WIND_CLASS_TYPE type, HINSTANCE resHandle, WNDPROC wndProc)
	{
		WindowClass* wndClass;
		if (_sRegisteredClasses.find(type) == _sRegisteredClasses.end())
		{
			wndClass = CreateClass(type, resHandle, wndProc);
			ASSERT(wndClass != nullptr);

			_sRegisteredClasses[type] = wndClass;
		}
		else
		{
			wndClass = GetClass(type);
		}

		wndClass->Register();

		return _sRegisteredClasses[type]->GetName();
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

	WindowClass* WindowClassManager::CreateClass(WIND_CLASS_TYPE type, HINSTANCE resHandle, WNDPROC wndProc)
	{
		switch (type)
		{
		case RE::WIND_CLASS_TYPE_MAIN:
			return new WindowClassMain(resHandle, wndProc);
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
#pragma once

#include "pch.h"
#include <map>

namespace RE
{
#define REGISTER_CLASS(name) 
#define CLASS_MAIN_NAME L"RyEngine"

	typedef void (*WindowEventHandler)(UINT, WPARAM, LPARAM);
	typedef void (*InputEventHandler)(UINT, WPARAM, LPARAM);

	enum WIND_CLASS_TYPE
	{
		WIND_CLASS_TYPE_NONE = 0x00,
		WIND_CLASS_TYPE_MAIN = 0x01
	};

	class WindowClass
	{
	public:
		WindowClass(const wchar_t* name);

		virtual ~WindowClass() {}

		void Register() { _mWndRefCount++; }
		virtual void Release(HINSTANCE resHandle);

		const wchar_t* GetName() const { return _mName; }
		bool IsRegistered() const { return _mWndRefCount > 0; }

	protected:
		const wchar_t* _mName;
		const char* _mMenuName;
		uint16_t _mWndRefCount;
		UINT _mSize;
		UINT _mStyle;
		HICON _mIcon;
		HICON _mIconSm;
		HCURSOR _mCursor;
		HBRUSH _mBackground;
		//WindowEventHandler _mWindowEvent;
		//InputEventHandler _mInputEvent;

		void InternalRegister(HINSTANCE resHandle);

	private:
		static LRESULT CALLBACK WindowProcSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

	class WindowClassMain : public WindowClass
	{
	public:
		WindowClassMain(HINSTANCE resHandle) :
			WindowClass(CLASS_MAIN_NAME)
		{
			_mSize = sizeof(WNDCLASSEX);
			_mStyle = CS_HREDRAW | CS_VREDRAW;
			_mIcon = LoadIcon(resHandle, MAKEINTRESOURCE(IDI_RYENGINE));
			_mIconSm = LoadIcon(resHandle, MAKEINTRESOURCE(IDI_SMALL));
			_mCursor = LoadCursor(nullptr, IDC_ARROW);
			_mBackground = (HBRUSH)(COLOR_WINDOW + 1);

			InternalRegister(resHandle);
		}
	};

	class WindowClassManager
	{
	public:
		static WindowClass* RegisterWindowClass(WIND_CLASS_TYPE type, HINSTANCE resHandle);
		static void ReleaseWindowClass(WIND_CLASS_TYPE type, HINSTANCE resHandle);

	private:
		static std::map<WIND_CLASS_TYPE, WindowClass*> _sRegisteredClasses;

		static WindowClass* CreateClass(WIND_CLASS_TYPE type, HINSTANCE resHandle);
		static WindowClass* GetClass(WIND_CLASS_TYPE type);
	};
}
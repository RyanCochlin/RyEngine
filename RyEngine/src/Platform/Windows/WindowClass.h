#pragma once

#include "pch.h"
#include <map>

namespace RE
{
#define REGISTER_CLASS(name) 
#define CLASS_MAIN_NAME "RyEngine"

	enum WIND_CLASS_TYPE
	{
		WIND_CLASS_TYPE_NONE = 0x00,
		WIND_CLASS_TYPE_MAIN = 0x01
	};

	class WindowClass
	{
	public:
		WindowClass(const char* name);

		virtual ~WindowClass() {}

		void Register() { _mWndRefCount++; }
		virtual void Release(HINSTANCE resHandle);

		const char* GetName() const { return _mName; }
		bool IsRegistered() const { return _mWndRefCount > 0; }

	protected:
		const char* _mName;
		const char* _mMenuName;
		uint16_t _mWndRefCount;
		UINT _mSize;
		UINT _mStyle;
		HICON _mIcon;
		HICON _mIconSm;
		HCURSOR _mCursor;
		HBRUSH _mBackground;

		void InternalRegister(HINSTANCE resHandle, WNDPROC wndProc);
	};

	class WindowClassMain : public WindowClass
	{
	public:
		WindowClassMain(HINSTANCE resHandle, WNDPROC wndProc) :
			WindowClass(CLASS_MAIN_NAME)
		{
			_mSize = sizeof(WNDCLASSEX);
			_mStyle = CS_HREDRAW | CS_VREDRAW;
			_mIcon = LoadIcon(resHandle, MAKEINTRESOURCE(IDI_RYENGINE));
			_mIconSm = LoadIcon(resHandle, MAKEINTRESOURCE(IDI_SMALL));
			_mCursor = LoadCursor(nullptr, IDC_ARROW);
			_mBackground = (HBRUSH)(COLOR_WINDOW + 1);

			InternalRegister(resHandle, wndProc);
		}
	};

	class WindowClassManager
	{
	public:
		static const char* RegisterWindowClass(WIND_CLASS_TYPE type, HINSTANCE resHandle, WNDPROC wndProc);
		static void ReleaseWindowClass(WIND_CLASS_TYPE type, HINSTANCE resHandle);

	private:
		static std::map<WIND_CLASS_TYPE, WindowClass*> _sRegisteredClasses;

		static WindowClass* CreateClass(WIND_CLASS_TYPE type, HINSTANCE resHandle, WNDPROC wndProc);
		static WindowClass* GetClass(WIND_CLASS_TYPE type);
	};
}
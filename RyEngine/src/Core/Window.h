#pragma once

#define STRING_BUFFER_MAX 100

namespace RE
{
	class Window
	{
	public:
		Window(HINSTANCE hInst, int cmdShow, LPWSTR winClass, LPWSTR winTitle, UINT w, UINT h) : _width(w), _height(h)
		{
			
			_hWnd = CreateWindow(winClass,
				winTitle,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				_width,
				_height,
				nullptr,
				nullptr,
				hInst,
				nullptr);

			ShowWindow(_hWnd, cmdShow);
			UpdateWindow(_hWnd);
		}

		HWND get_hWND();

	private:
		HWND _hWnd;
		UINT _width;
		UINT _height;
	};
}


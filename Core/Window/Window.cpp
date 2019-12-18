#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Window.h"

namespace RyEngine
{
	HWND Window::get_hWND()
	{
		return _hWnd;
	}
}
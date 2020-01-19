// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#if _DEBUG

#define _CRTDBG_MAP_ALLOC

#endif // _DEBUG

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#if _DEBUG

#include <crtdbg.h>

#endif // _DEBUG

// reference additional headers your program requires here

//*******************Direct3D headers and libraries

#include <wrl.h>
#include <d3d12.h>
#include <DXGI1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "d3dx12.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

//*******************End Direct3D headers and libraries

#include "Resource.h"

#include "GlobalDefines.h"
#include "AssertDefines.h"
#include "Error.h"
#include "AssertMain.h"
#include "AssertStatic.h"
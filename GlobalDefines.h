//Ryan Cochlin - 7/13/2019
//Assertions Main
//TODO document
#pragma once

#if _DEBUG

#define DEBUG _DEBUG

#define ENABLE_ASSERT true
#define S_ASSERT_TYPE_DIV true

#else

#define DISABLE_ASSERT true

#endif

#ifndef __ry_graphics_api__
#define __ry_graphics_api__

#define RYGRAPHICS_API_DIRECTX 1

#define CURRENT_GRAPHICS_API RYGRAPHICS_API_DIRECTX;

#endif // !__ry_graphics_api__

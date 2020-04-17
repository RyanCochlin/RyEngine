//Ryan Cochlin - 7/13/2019
//Assertions Main
//TODO document
#pragma once


#ifdef RE_WINDOWS
	#ifdef RE_DLL_BUILD
		#define RE_API __declspec(dllexport)
	#else
		#define RE_API _declspec(dllimport)
	#endif // RE_DLL_BUILD
#endif // RE_WINDOWS

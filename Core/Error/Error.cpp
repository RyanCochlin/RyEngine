//Ryan Cochlin - 7/13/2019
//Assertions Main
//TODO document

#include "ErrorLog.cpp"
#include "RyException.h"
#include "DirectX/RyDXException.h"
#include "../Utils.cpp"

//TODO get the exception type by platform

#ifndef ThrowIfFailed
#define ThrowIfFailed(x) \
{ \
	HRESULT hr__ = (x); \
	std::wstring wfn = AnsiToWString(__FILE__); \
	if(FAILED(hr__)) { throw RyError::RyDXException(hr__, L#x, wfn, __LINE__); } \
}
#endif
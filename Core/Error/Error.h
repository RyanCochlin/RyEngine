#pragma once

#include "ErrorLog.h"
#include "RyException.h"
#include "DirectX/RyDXException.h"
#include "Utils.h"

#ifndef ThrowIfFailed
#define ThrowIfFailed(x) \
{ \
	HRESULT hr__ = (x); \
	std::wstring wfn = AnsiToWString(__FILE__); \
	if(FAILED(hr__)) { throw RyError::RyDXException(hr__, L#x, wfn, __LINE__); } \
}
#endif
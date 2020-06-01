#pragma once

#include <iostream>

#include "Log.h"
#include "Exception/RyException.h"
#include "Exception/RyDXException.h"
#include "Utils.h"

#ifndef ThrowIfFailed
#define ThrowIfFailed(x) \
{ \
	HRESULT hr__ = (x); \
	if(FAILED(hr__)) { throw RE::RyDXException(hr__, __FILE__, __LINE__); } \
}
#endif

inline void ReportAssertionFail(const char* assertType, const char* message, const char* file, int line)
{
	RE_CORE_ERROR("Assert Fail in {0} ({1}): {2} {3}", file, line, assertType, message);
}
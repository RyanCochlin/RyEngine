#pragma once

#include <iostream>

#include "RyException.h"
#include "RyDXException.h"
#include "Utils.h"

#ifndef ThrowIfFailed
#define ThrowIfFailed(x) \
{ \
	HRESULT hr__ = (x); \
	std::wstring wfn = AnsiToWString(__FILE__); \
	if(FAILED(hr__)) { throw RyError::RyDXException(hr__, L#x, wfn, __LINE__); } \
}
#endif

inline void ReportAssertionFail(const char* assertType, const char* message, const char* file, int line)
{
	//TODO print to error log. For now just output to console
	std::cerr << "Assert Fail in " << file << " line " << line << ": " << assertType << " " << message;
}
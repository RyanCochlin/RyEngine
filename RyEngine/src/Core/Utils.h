#pragma once

#include "pch.h"

#include <string>

#ifndef _UTILS_
#define _UTILS_

inline std::wstring AnsiToWString(const std::string& str)
{
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
	return std::wstring(buffer);
}

#endif
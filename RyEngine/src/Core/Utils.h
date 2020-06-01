#pragma once

#include "pch.h"

#include <string>

#ifndef _UTILS_
#define _UTILS_

namespace RE
{
	inline std::wstring AnsiToWString(const std::string& str)
	{
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
		return std::wstring(buffer);
	}

	inline std::wstring AnsiToWString(const char* str)
	{
		std::string s(str);
		return AnsiToWString(s);
	}

	inline std::string WCharPToString(wchar_t* wCharP)
	{
		std::wstring ws(wCharP);
		std::string str(ws.begin(), ws.end());
		return str;
	}
}
#endif
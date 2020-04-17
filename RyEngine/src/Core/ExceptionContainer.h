#pragma once

#include "pch.h"

#include <string>

namespace RyError
{
	class ExceptionContainer
	{
	public:
		ExceptionContainer(const std::wstring& functionName, const std::wstring& fileName, int lineNumber) : functionName(functionName), fileName(fileName), lineNumber(lineNumber) {}
		virtual ~ExceptionContainer() {}

		virtual const std::wstring ToString() = 0;

	protected:
		std::wstring functionName;
		std::wstring fileName;
		int lineNumber = -1;
	};
}
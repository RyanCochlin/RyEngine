#pragma once

#include "stdafx.h"

#include "winerror.h"
#include "ExceptionContainer.h"

#pragma once

namespace RyError
{
	class RyDXException : public ExceptionContainer
	{
	public:
		RyDXException(HRESULT hr, const std::wstring& functionName, const std::wstring& fileName, int lineNumber) : ExceptionContainer(functionName, fileName, lineNumber), _hr(hr) {}
		~RyDXException() {}

		const std::wstring ToString()
		{
			return fileName + L"::" + functionName + L"() Line: " + std::to_wstring(lineNumber);
		}
	protected:
		HRESULT _hr;
	};
}


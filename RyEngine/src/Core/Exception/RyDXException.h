#pragma once

#include <Windows.h>
#include <system_error>
#include <string>

#include "RyException.h"

#pragma once

namespace RE
{
	class RyDXException : public RyException
	{
	public:
		RyDXException(HRESULT hr, char* fileName, uint32_t lineNumber) :
			RyException(fileName, lineNumber),
			_mHResult(hr)
		{}

		std::string ToString() override
		{
			std::string err;
			err = RyException::ToString() + ": DirectX Exception: HRESULT: " + std::to_string(_mHResult) + ": " + std::system_category().message(_mHResult);
			return err;
		}

	protected:
		HRESULT _mHResult;
	};
}


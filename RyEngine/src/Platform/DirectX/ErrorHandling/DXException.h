#pragma once

#include <string>

#include "Core/Utils.h"
#include "Core/Exception/RyException.h"

#pragma once

#define RE_CATCH_DX } catch(RE::DXException e) { RE_CORE_ERROR(e.ToString()); }

namespace RE
{
	class DXException : public RyException
	{
	public:
		DXException(HRESULT hr, char* fileName, uint32_t lineNumber) :
			RyException(fileName, lineNumber),
			_mHResult(hr),
			_dxErrBuf(nullptr)
		{}

		std::string ToString() override
		{
			std::string err;
			if (GetErrorString())
			{
				err = RyException::ToString() + "\nDirectX Exception: " + WCharPToString(_dxErrBuf);// GetErrorString();
			}
			return err;
		}

	private:
		static const DWORD _dwChars = 512;

		HRESULT _mHResult;
		LPWSTR _dxErrBuf;


		DWORD GetErrorString()
		{
			return FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				_mHResult,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPWSTR)&_dxErrBuf,
				0,
				NULL
			);
		}
	};
}


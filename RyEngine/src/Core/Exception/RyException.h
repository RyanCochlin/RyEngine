#pragma once

#include <string>
#include "Core/Log.h"

#define RE_TRY try{
#define RE_CATCH } catch(RE::RyException e) { RE_CORE_ERROR(e.ToString()); }
#define RE_CATCH_DX } catch(RE::RyDXException e) { RE_CORE_ERROR(e.ToString()); }

namespace RE
{
	class RyException
	{
	public:
		RyException(char* fileName, uint32_t lineNumber) :
			_mFileName(fileName),
			_mLineNumber(lineNumber)
		{}

		virtual std::string ToString()
		{
			std::string err;
			err = "Exception Thrown: File: " + std::string(_mFileName) + ", Line: " + std::to_string(_mLineNumber);
			return err;
		}

	protected:
		char* _mFileName;
		uint32_t _mLineNumber;
	};
}
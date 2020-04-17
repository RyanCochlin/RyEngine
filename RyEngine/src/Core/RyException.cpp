#include "pch.h"
#include "RyException.h"

namespace RyError
{
	RyException::~RyException()
	{
		delete _exception;
	}

	std::wstring RyException::ToString()
	{
		return _exception->ToString();
	}
}
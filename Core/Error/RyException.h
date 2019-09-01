#include "ExceptionContainer.h"

#pragma once

namespace RyError
{
	class RyException
	{
	public:
		RyException() = default;
		RyException(RyError::ExceptionContainer* exception) : _exception(exception) {}
		~RyException();

		std::wstring ToString();

	private:
		RyError::ExceptionContainer* _exception;
	};
}
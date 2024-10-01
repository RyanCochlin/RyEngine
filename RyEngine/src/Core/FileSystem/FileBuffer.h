
#pragma once

#include "pch.h"

namespace RE
{

RE_API struct RFSFileBuffer
{
	void* buffer;
	size_t size;

	~RFSFileBuffer()
	{
		delete buffer;
	}

	const char* c_str()
	{
		auto* strBuffer = static_cast<char*>(buffer);
		strBuffer[size] = '\0';
		return strBuffer;
	}
};

}
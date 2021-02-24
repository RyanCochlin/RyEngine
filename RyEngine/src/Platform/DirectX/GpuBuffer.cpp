#include "pch.h"
#include "GpuBuffer.h"
#include "Core/Math/CoreMath.h"

namespace RE
{
	void GpuBuffer::Create()
	{

	}

	void GpuBuffer::CreateConstaintBufferView(uint32_t offset, uint32_t size)
	{
		size = Math::Align(size, 16);
	}
}
#pragma once
#include "GpuResource.h"

namespace RE
{
	class GpuBuffer : public GpuResource
	{
	public:
		void Create();
		void CreateConstaintBufferView(uint32_t offset, uint32_t size);
	};
}


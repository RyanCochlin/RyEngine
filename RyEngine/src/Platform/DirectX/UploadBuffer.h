#pragma once

#include "GpuResource.h"

namespace RE
{
	class UploadBuffer : public GpuResource
	{
	public:
		UploadBuffer();

		void Create(ID3D12Device* device, UINT numElements, UINT elementSize);
		void* Map();
		void Unmap();

	private:
		ID3D12Device* _mDevice;
		void* _mCpuVirtualAddress;
	};
}
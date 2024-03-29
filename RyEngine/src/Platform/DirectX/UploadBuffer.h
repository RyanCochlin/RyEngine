#pragma once

#include "GpuResource.h"

namespace RE
{
	class UploadBuffer : public GpuResource
	{
	public:
		UploadBuffer();

		void Create(UINT numElements, UINT elementSize);
		void CreateConstaintBufferView(uint32_t offset, uint32_t size, ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cbvDescriptorHandle);
		void* Map();
		void Unmap();
		void UploadResource(ID3D12GraphicsCommandList* commandList, GpuResource* resource, D3D12_SUBRESOURCE_DATA data);

	private:
		void* _mCpuVirtualAddress;
	};
}
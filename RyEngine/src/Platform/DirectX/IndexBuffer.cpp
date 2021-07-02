#include "pch.h"
#include "IndexBuffer.h"

namespace RE
{
	IndexBuffer::IndexBuffer() : 
		_mIndexStride(sizeof(UINT)),
		_mCount(0)
	{}

	IndexBuffer::~IndexBuffer()
	{}

	void IndexBuffer::Create(ID3D12Device* device, UINT numElements, UINT elementSize)
	{
		UINT byteSize = numElements * elementSize;

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&_mResource)));
	}

	D3D12_GPU_VIRTUAL_ADDRESS IndexBuffer::GetGpuAddress()
	{
		return _mResource->GetGPUVirtualAddress();
	}
}
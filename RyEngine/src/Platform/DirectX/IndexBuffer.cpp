#include "pch.h"
#include "IndexBuffer.h"
#include "DirectXCore.h"

namespace RE
{
	IndexBuffer::IndexBuffer(std::vector<RE_INDEX>& indicies)
	{
		_mIndicies = indicies;
		_mIndexStride = sizeof(RE_INDEX);
		_mCount = _mIndicies.size();
		Create();
	}

	IndexBuffer::IndexBuffer(std::vector<RE_INDEX>&& indicies)
	{
		_mIndicies = indicies;
		_mIndexStride = sizeof(RE_INDEX);
		_mCount = _mIndicies.size();
		Create();
	}

	void IndexBuffer::Create()
	{
		UINT byteSize = _mIndexStride * _mCount;

		ThrowIfFailed(DirectXCore::GetDevice()->CreateCommittedResource(
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
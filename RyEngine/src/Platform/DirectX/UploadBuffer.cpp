#include "pch.h"
#include "UploadBuffer.h"
#include "MeshGeometry.h"

namespace RE
{
	UploadBuffer::UploadBuffer() :
		_mCpuVirtualAddress(nullptr)
	{
		_mGpuVirtualAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;
	}

	void UploadBuffer::Create(ID3D12Device* device, UINT numElements, UINT elementSize)
	{
		UINT size = numElements * elementSize;

		HRESULT hr = device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(size),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_mResource)
		);

		if (FAILED(hr))
		{
			ThrowIfFailed(hr);
		}

		_mGpuVirtualAddress = _mResource->GetGPUVirtualAddress();
	}

	void* UploadBuffer::Map()
	{
		CD3DX12_RANGE range(0, 0);
		ThrowIfFailed(_mResource->Map(0, &range, &_mCpuVirtualAddress));
		return _mCpuVirtualAddress;
	}

	void UploadBuffer::Unmap()
	{
		_mResource->Unmap(0, nullptr);
	}
}
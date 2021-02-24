#include "pch.h"
#include "UploadBuffer.h"
#include "MeshGeometry.h"
#include "DirectXCore.h"
#include "Core/Math/CoreMath.h"

namespace RE
{
	UploadBuffer::UploadBuffer() :
		_mCpuVirtualAddress(nullptr)
	{
		_mGpuVirtualAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;
	}

	void UploadBuffer::Create(UINT numElements, UINT elementSize)
	{
		UINT size = numElements * elementSize;
		ID3D12Device* device = DirectXCore::GetDevice();

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

	void UploadBuffer::CreateConstaintBufferView(uint32_t offset, uint32_t size, ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cbvDescriptorHandle)
	{
		size = Math::Align(size, 16);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = _mGpuVirtualAddress + (size_t)offset;
		cbvDesc.SizeInBytes = size;

		device->CreateConstantBufferView(&cbvDesc, cbvDescriptorHandle);
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
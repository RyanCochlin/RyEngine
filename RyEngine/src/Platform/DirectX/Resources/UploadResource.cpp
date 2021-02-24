#include "pch.h"
#include "UploadResource.h"
#include "Platform/DirectX/DirectXCore.h"
#include "Platform/DirectX/UploadBuffer.h"

namespace RE
{
	UploadResource::UploadResource() :
		_mSize(0),
		_mUploadBuffer{}
	{
		_mUploadBuffer = new UploadBuffer();
	}

	UploadResource::~UploadResource()
	{
		delete _mUploadBuffer;
	}

	void UploadResource::Create()
	{
		ASSERT(_mSize > 0);
		_mCBV = DirectXCore::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		_mUploadBuffer->Create(_mCount, _mSize);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = _mUploadBuffer->GetGpuVirtualAddress(); //TODO add offset in the future
		cbvDesc.SizeInBytes = _mSize;
		DirectXCore::GetDevice()->CreateConstantBufferView(&cbvDesc, _mCBV);
	}
}
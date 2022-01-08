#pragma once

#include "Platform/DirectX/UploadBuffer.h"

namespace RE
{
	template<typename T>
	class UploadResource
	{
	public:
		UploadResource() : 
			_mSize(0),
			_mUploadBuffer{},
			_mCpuBuffer(nullptr)
		{
			_mUploadBuffer = new UploadBuffer();
			_mDataSize = sizeof(T);
		}

		~UploadResource()
		{
			_mUploadBuffer->Unmap();
			delete _mUploadBuffer;
		}

		void Create()
		{
			_mSize = sizeof(T);
			_mCount = 1;
			ASSERT(_mSize > 0);

			_mUploadBuffer->Create(_mCount, _mSize);
			_mCBV = DirectXCore::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = _mUploadBuffer->GetGpuVirtualAddress(); // TODO this needs to be offset
			cbvDesc.SizeInBytes = Math::Align(_mSize, 256);
			DirectXCore::GetDevice()->CreateConstantBufferView(&cbvDesc, _mCBV);
			_mCpuBuffer = _mUploadBuffer->Map();
		}

		void Upload(T& cb)
		{
			memcpy(_mCpuBuffer, &cb, _mDataSize);
		}

		void SetDescriptor(CommandContext* commandContext, UINT index, uint32_t offset)
		{
			ComPtr<ID3D12DescriptorHeap> heap = DirectXCore::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			UINT incrementSize = DirectXCore::GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			auto handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(heap->GetGPUDescriptorHandleForHeapStart());
			handle.Offset(offset, incrementSize);
			commandContext->SetDescriptorTable(index, handle);
		}

		void GetCpuHandle();
		void GetGpuHandle();

	protected:
		void* _mCpuBuffer;
		UploadBuffer* _mUploadBuffer;
		D3D12_CPU_DESCRIPTOR_HANDLE _mCBV;
		size_t _mSize;
		size_t _mCount;
		size_t _mDataSize;
	};
}
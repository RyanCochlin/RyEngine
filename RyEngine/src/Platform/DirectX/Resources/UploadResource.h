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

		void Create(uint32_t count)
		{
			_mSize = sizeof(T);
			_mCount = count;
			ASSERT(_mSize > 0);

			_mUploadBuffer->Create(_mCount, _mSize);
			_mCpuBuffer = _mUploadBuffer->Map();
			//_mCBV = DirectXCore::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

			size_t alignedSize = Math::Align(_mSize, 256);
			for (uint32_t i = 0; i < count; ++i)
			{
				D3D12_CPU_DESCRIPTOR_HANDLE handle = DirectXCore::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

				D3D12_GPU_VIRTUAL_ADDRESS cbAddress = _mUploadBuffer->GetGpuVirtualAddress();
				cbAddress += (i * alignedSize);

				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
				cbvDesc.BufferLocation = cbAddress;
				cbvDesc.SizeInBytes = alignedSize;
				DirectXCore::GetDevice()->CreateConstantBufferView(&cbvDesc, handle);
			}
		}

		void Upload(T& cb, int index)
		{
			size_t size = Math::Align(_mSize, 256);
			memcpy((char*)_mCpuBuffer+(index * size), &cb, _mDataSize);
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
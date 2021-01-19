#pragma once

#include <vector>

using Microsoft::WRL::ComPtr;

namespace RE
{
	class DescriptorAllocatorPage;

	class DescriptorAllocator
	{
	public:
		DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type);

		D3D12_CPU_DESCRIPTOR_HANDLE Allocate(ID3D12Device* device, uint32_t count);
		void ReleaseStale(uint64_t frame);

	private:
		//TODO make this a heap for faster search
		//using DescriptorHeapPool = std::vector<DescriptorAllocatorPage*>;

		const int DESCRIPTORS_PER_HEAP = 256;

		D3D12_DESCRIPTOR_HEAP_TYPE _mType;
		std::vector<DescriptorAllocatorPage*> _mDescriptorHeapPool;
		uint32_t _mCurrentHeapIndex;
		uint32_t _mDescriptorSize;
		uint32_t _mRemainingFree;
		D3D12_CPU_DESCRIPTOR_HANDLE _mCurrentHandle;
	};

	class DescriptorAllocatorPage
	{
	public:
		DescriptorAllocatorPage(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorSize);

		inline uint32_t GetRemainingSize() const { return _mRemainingSize; };
		D3D12_CPU_DESCRIPTOR_HANDLE Allocate(uint32_t count);
		void Clear();

	private:
		ID3D12Device* _mDevice;
		D3D12_DESCRIPTOR_HEAP_TYPE _mType;
		ID3D12DescriptorHeap* _mDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE _mCurrentHandle;
		uint32_t _mDescriptorSize;
		uint32_t _mRemainingSize;
	};
}
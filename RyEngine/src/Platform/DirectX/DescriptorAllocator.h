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

		D3D12_CPU_DESCRIPTOR_HANDLE Allocate(uint32_t count, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
		ComPtr<ID3D12DescriptorHeap> GetCurrentDescriptorHeap();
		UINT GetDescriptorIncrementSize();
		void ReleaseStale(uint64_t frame);

		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocatorTest();

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

		ComPtr<ID3D12DescriptorHeap> _mTestHeap;
	};

	class DescriptorAllocatorPage
	{
	public:
		DescriptorAllocatorPage(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		uint32_t GetRemainingSize() const { return _mRemainingDescriptors; };
		ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const { return _mDescriptorHeap; }
		D3D12_CPU_DESCRIPTOR_HANDLE Allocate(uint32_t count);
		UINT GetDescriptorIncrementSize();
		void Clear();

	private:
		D3D12_DESCRIPTOR_HEAP_TYPE _mType;
		ComPtr<ID3D12DescriptorHeap> _mDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE _mCurrentHandle;
		uint32_t _mNumDescriptors;
		uint32_t _mRemainingDescriptors;
		uint32_t _mDescriptorSize;
		uint32_t _mCurrentOffset;
	};
}
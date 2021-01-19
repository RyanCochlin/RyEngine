#include "pch.h"
#include "DescriptorAllocator.h"
#include "Core/Assert.h"

namespace RE
{
	//-----------------------------DescriptorAllocator-----------------------------//
	DescriptorAllocator::DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type) :
		_mType(type),
		_mCurrentHeapIndex(0),
		_mDescriptorSize(0),
		_mRemainingFree(0),
		_mCurrentHandle()
	{}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::Allocate(ID3D12Device* device, uint32_t count)
	{
		//TODO: add mutex lock for multithreading

		DescriptorAllocatorPage* nextPage = nullptr;

		std::vector<DescriptorAllocatorPage*>::iterator iter = _mDescriptorHeapPool.begin();
		for (; iter != _mDescriptorHeapPool.end(); ++iter)
		{
			//find first heap with enough space
			DescriptorAllocatorPage* allocatorPage = *iter;
			if (allocatorPage != nullptr && allocatorPage->GetRemainingSize() > count)
			{
				nextPage = allocatorPage;
				break;
			}
		}

		if (nextPage == nullptr)
		{
			//no allocator page found. Allocate new
			DescriptorAllocatorPage* newPage = new DescriptorAllocatorPage(device, _mType, DESCRIPTORS_PER_HEAP);
			_mDescriptorHeapPool.push_back(newPage);
			nextPage = newPage;
		}

		return nextPage->Allocate(count);
	}

	void DescriptorAllocator::ReleaseStale(uint64_t frame)
	{

	}

	//-----------------------------DescriptorAllocatorPage------------------------//
	DescriptorAllocatorPage::DescriptorAllocatorPage(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorSize) :
		_mDevice(device),
		_mType(type),
		_mDescriptorSize(descriptorSize),
		_mRemainingSize(descriptorSize)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		desc.Type = _mType;
		desc.NumDescriptors = descriptorSize;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 1;
		ThrowIfFailed(_mDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_mDescriptorHeap)));
		_mCurrentHandle = _mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocatorPage::Allocate(uint32_t count)
	{
		ASSERT(count <= _mRemainingSize);

		if (_mDescriptorSize == 0)
		{
			_mDescriptorSize = _mDevice->GetDescriptorHandleIncrementSize(_mType);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE cur = _mCurrentHandle;
		_mCurrentHandle.ptr += count * _mDescriptorSize;
		_mRemainingSize -= count;
		return cur;
	}

	void DescriptorAllocatorPage::Clear()
	{
		_mDescriptorHeap = nullptr;
		_mCurrentHandle.ptr = 0;
		_mRemainingSize = _mDescriptorSize;
		_mCurrentHandle = _mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}
}
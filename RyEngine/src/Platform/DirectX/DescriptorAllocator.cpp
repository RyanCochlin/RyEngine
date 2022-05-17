#include "pch.h"
#include "DescriptorAllocator.h"
#include "Core/Assert.h"
#include "DirectXCore.h"

namespace RE
{
	//-----------------------------DescriptorAllocator-----------------------------//
	DescriptorAllocator::DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type) :
		_mType(type),
		_mCurrentHeapIndex(0),
		_mDescriptorSize(0),
		_mRemainingFree(0),
		_mTotalAllocations(0),
		_mCurrentHandle()
	{}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::Allocate(uint32_t count, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		//TODO: add mutex lock for multithreading

		DescriptorAllocatorPage* nextPage = nullptr;
		for(auto& allocatorPage : _mDescriptorHeapPool)
		{
			//find first heap with enough space
			if (allocatorPage != nullptr && allocatorPage->GetRemainingSize() > count)
			{
				nextPage = allocatorPage;
				break;
			}
		}

		if (nextPage == nullptr)
		{
			//no allocator page found. Allocate new
			DescriptorAllocatorPage* newPage = new DescriptorAllocatorPage(_mType, DESCRIPTORS_PER_HEAP, flags);
			_mDescriptorHeapPool.push_back(newPage);
			nextPage = newPage;
		}
		_mTotalAllocations += count;

		return nextPage->Allocate(count);
	}

	ComPtr<ID3D12DescriptorHeap> DescriptorAllocator::GetCurrentDescriptorHeap()
	{
		ComPtr<ID3D12DescriptorHeap> heap;
		DescriptorAllocatorPage* page;
		if (_mDescriptorHeapPool.size() > 0)
		{
			page = _mDescriptorHeapPool.back();
			heap = page->GetDescriptorHeap();
		}
		return heap;// (_mDescriptorHeapPool.size() > 0) ? _mDescriptorHeapPool.back()->GetDescriptorHeap() : nullptr;
	}

	UINT DescriptorAllocator::GetDescriptorIncrementSize()
	{
		UINT incrementSize = 0;
		DescriptorAllocatorPage* page;
		if (_mDescriptorHeapPool.size() > 0)
		{
			page = _mDescriptorHeapPool.back();
			incrementSize = page->GetDescriptorIncrementSize();
		}

		return incrementSize;
	}

	uint32_t DescriptorAllocator::GetTotalAllocations()
	{
		return _mTotalAllocations;
	}

	void DescriptorAllocator::ReleaseStale(uint64_t frame)
	{
		//TODO need to release descriptors. I'm leaking memory
	}

	//-----------------------------DescriptorAllocatorPage------------------------//
	DescriptorAllocatorPage::DescriptorAllocatorPage(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags) :
		_mType(type),
		_mNumDescriptors(numDescriptors),
		_mRemainingDescriptors(numDescriptors),
		_mDescriptorSize(0),
		_mCurrentOffset(0)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		desc.Type = _mType;
		desc.NumDescriptors = _mNumDescriptors;
		desc.Flags = flags;
		desc.NodeMask = 0; //TODO sort out the node mask for multi adapter
		ThrowIfFailed(DirectXCore::GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_mDescriptorHeap)));
		_mCurrentHandle = _mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocatorPage::Allocate(uint32_t count)
	{
		ASSERT(count <= _mRemainingDescriptors);

		if (_mDescriptorSize == 0)
		{
			_mDescriptorSize = DirectXCore::GetDevice()->GetDescriptorHandleIncrementSize(_mType);
		}

		// TODO need to keep track of current descriptor offset
		D3D12_CPU_DESCRIPTOR_HANDLE cur = _mCurrentHandle;
		_mCurrentOffset = count * _mDescriptorSize;
		_mCurrentHandle.ptr += _mCurrentOffset;
		_mRemainingDescriptors -= count;
		return cur;
	}

	UINT DescriptorAllocatorPage::GetDescriptorIncrementSize()
	{
		if (_mDescriptorSize == 0)
		{
			_mDescriptorSize = DirectXCore::GetDevice()->GetDescriptorHandleIncrementSize(_mType);
		}

		return _mDescriptorSize;
	}

	void DescriptorAllocatorPage::Clear()
	{
		_mDescriptorHeap = nullptr;
		_mCurrentHandle.ptr = 0;
		_mRemainingDescriptors = _mNumDescriptors;
		_mCurrentHandle = _mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}
}
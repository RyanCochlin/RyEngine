#include "pch.h"
#include "CommandAllocatorPool.h"

namespace RE
{
	CommandAllocatorPool::CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type) :
		_mType(type)
	{}

	CommandAllocatorPool::~CommandAllocatorPool()
	{
		Release();
	}

	void CommandAllocatorPool::Release()
	{
		for (size_t i = 0; i < _mAllocatorPool.size(); i++)
		{
			_mAllocatorPool[i]->Release();
		}

		_mAllocatorPool.clear();
	}

	ID3D12CommandAllocator* CommandAllocatorPool::Request(ID3D12Device* device, uint64_t fenceVal)
	{
		//TODO make thread safe

		ID3D12CommandAllocator* comAlloc = nullptr;
		if (!_mReadyAllocators.empty())
		{
			std::pair<uint64_t, ID3D12CommandAllocator*> allocPair = _mReadyAllocators.front();
			if (allocPair.first <= fenceVal)
			{
				comAlloc = allocPair.second;
				ThrowIfFailed(comAlloc->Reset());
				_mReadyAllocators.pop();
			}
		}

		if (comAlloc == nullptr)
		{
			ThrowIfFailed(device->CreateCommandAllocator(_mType, IID_PPV_ARGS(&comAlloc)));
			wchar_t allocName[32];
			swprintf(allocName, 32, L"CommandAllocator %zu", _mAllocatorPool.size());
			comAlloc->SetName(allocName);
			_mAllocatorPool.push_back(comAlloc);
		}

		return comAlloc;
	}

	void CommandAllocatorPool::Discard(ID3D12CommandAllocator* allocator, uint64_t fenceVal)
	{
		//TODO make thread safe
		_mReadyAllocators.push(std::make_pair(fenceVal, allocator));
	}
}
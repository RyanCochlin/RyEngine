#pragma once

#include <vector>
#include <queue>

namespace RE
{
	class CommandAllocatorPool
	{
	public:
		CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type);
		~CommandAllocatorPool();

		void Release();
		ID3D12CommandAllocator* Request(ID3D12Device* device, uint64_t fenceVal);
		void Discard(ID3D12CommandAllocator* allocator, uint64_t fenceVal);

	private:
		const D3D12_COMMAND_LIST_TYPE _mType;

		std::vector<ID3D12CommandAllocator*> _mAllocatorPool;
		std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>> _mReadyAllocators;
	};
}
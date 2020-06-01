#pragma once

#include "d3dx12.h"
#include "d3d12.h"
#include "dxgi.h"
#include "CommandAllocatorPool.h"

namespace RE
{
	//--------------------------CommandQueue-----------------------------//
	class CommandQueue
	{
	public:
		CommandQueue(D3D12_COMMAND_LIST_TYPE type);

		void Create(ID3D12Device* device);
		void Shutdown();

		ID3D12CommandQueue* GetCommandQueue() { return _mCommandQueue; }

	private:
		ID3D12CommandQueue* _mCommandQueue;
		ID3D12Fence* _mFence;
		D3D12_COMMAND_LIST_TYPE _mType;
		HANDLE _mFenceEventHandle;
		CommandAllocatorPool _mAllocatorPool;
		uint64_t _mNextFenceValue;
		uint64_t _mLastFenceValue;
	};

	//--------------------------CommandListManager-----------------------------//
	class CommandListManager
	{
	public:
		CommandListManager(ID3D12Device* device);

		void Shutdown();

		ID3D12CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);

	private:
		CommandQueue _mGraphicsCommandQueue;
	};
}

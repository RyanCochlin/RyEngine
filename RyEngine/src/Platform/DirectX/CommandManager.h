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

		ID3D12CommandAllocator* GetAllocator();
		uint64_t Execute(ID3D12CommandList* list);
		void ReleaseAllocator(ID3D12CommandAllocator* allocator);
		void WaitForFence(uint64_t fence);

		ID3D12CommandQueue* GetCommandQueue() { return _mCommandQueue; }

	private:
		ID3D12Device* _mDevice;
		ID3D12CommandQueue* _mCommandQueue;
		ID3D12Fence* _mFence;
		D3D12_COMMAND_LIST_TYPE _mType;
		HANDLE _mFenceEventHandle;
		CommandAllocatorPool _mAllocatorPool;
		uint64_t _mCurrentFenceValue;
	};

	//--------------------------CommandListManager-----------------------------//
	class CommandListManager
	{
	public:
		CommandListManager(ID3D12Device* device);

		void Shutdown();

		CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
		void CreateCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** list, ID3D12CommandAllocator** allocator);
		void WaitForFence(uint64_t fence);

	private:
		ID3D12Device* _mDevice;
		CommandQueue* _mGraphicsCommandQueue;
	};
}

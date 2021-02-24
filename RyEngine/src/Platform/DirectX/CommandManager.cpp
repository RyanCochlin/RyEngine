#include "pch.h"
#include "CommandManager.h"
#include "CommandAllocatorPool.h"

namespace RE
{
	//-----------CommandQueue-----------------//
	CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type) :
		_mType(type),
		_mCurrentFenceValue((uint64_t)type << 56),
		_mAllocatorPool(type)
	{}

	void CommandQueue::Create(ID3D12Device* device)
	{
		_mDevice = device;
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = _mType;
		queueDesc.NodeMask = 1;
		ThrowIfFailed(_mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_mCommandQueue)));

		ThrowIfFailed(_mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_mFence)));
		_mFenceEventHandle = CreateEvent(nullptr, false, false, nullptr);
	}

	ID3D12CommandAllocator* CommandQueue::GetAllocator()
	{
		return _mAllocatorPool.Request(_mDevice, _mCurrentFenceValue);
	}

	uint64_t CommandQueue::Execute(ID3D12CommandList* list)
	{
		ThrowIfFailed(((ID3D12GraphicsCommandList*)list)->Close());
		_mCommandQueue->ExecuteCommandLists(1, &list);
		_mCurrentFenceValue++;
		_mCommandQueue->Signal(_mFence, _mCurrentFenceValue);
		return _mCurrentFenceValue;
	}

	void CommandQueue::ReleaseAllocator(ID3D12CommandAllocator* allocator)
	{
		_mAllocatorPool.Discard(allocator, _mCurrentFenceValue);
	}

	void CommandQueue::WaitForFence(uint64_t fence)
	{
		UINT64 completedFence = _mFence->GetCompletedValue();
		if (completedFence < _mCurrentFenceValue)
		{
			_mFence->SetEventOnCompletion(_mCurrentFenceValue, _mFenceEventHandle);
			WaitForSingleObject(_mFenceEventHandle, INFINITE);
		}
	}

	void CommandQueue::Shutdown()
	{
		_mCommandQueue->Release();
	}

	//---------------CommandListManager---------//
	CommandListManager::CommandListManager(ID3D12Device* device) :
		_mDevice(device)
	{
		_mGraphicsCommandQueue = new CommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
		_mGraphicsCommandQueue->Create(device);
	}

	void CommandListManager::Shutdown()
	{
		_mGraphicsCommandQueue->Shutdown();
		delete _mGraphicsCommandQueue;
	}

	CommandQueue* CommandListManager::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type)
	{
		switch (type)
		{
		//TODO: Implement other command list types
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
		default:
			return _mGraphicsCommandQueue;
		}
	}

	void CommandListManager::CreateCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** list, ID3D12CommandAllocator** allocator)
	{
		//TODO this is creating a second allocator instead of using the one passed in
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
			*allocator = _mGraphicsCommandQueue->GetAllocator();
			break;
		}

		//TODO: deal with node mask someday
		//TODO: what to do about init pipeline state?
		ThrowIfFailed(_mDevice->CreateCommandList(0, type, *allocator, nullptr, IID_PPV_ARGS(list)));
	}

	void CommandListManager::WaitForFence(uint64_t fence)
	{
		_mGraphicsCommandQueue->WaitForFence(fence);
	}
}
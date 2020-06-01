#include "pch.h"
#include "CommandManager.h"

namespace RE
{
	//-----------CommandQueue-----------------//
	CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type) :
		_mType(type),
		_mNextFenceValue((uint64_t)type << 56 | 1),
		_mLastFenceValue((uint64_t)type << 56),
		_mAllocatorPool(type)
	{}

	void CommandQueue::Create(ID3D12Device* device)
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = _mType;
		queueDesc.NodeMask = 1;
		ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_mCommandQueue)));

		ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_mFence)));
		_mFence->Signal((uint64_t)_mType << 56);

		_mFenceEventHandle = CreateEvent(nullptr, false, false, nullptr);
	}

	void CommandQueue::Shutdown()
	{
		_mCommandQueue->Release();
	}

	//---------------CommandListManager---------//
	CommandListManager::CommandListManager(ID3D12Device* device) :
		_mGraphicsCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT)
	{
		_mGraphicsCommandQueue.Create(device);
	}

	void CommandListManager::Shutdown()
	{
		_mGraphicsCommandQueue.Shutdown();
	}

	ID3D12CommandQueue* CommandListManager::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type)
	{
		switch (type)
		{
		//TODO: Implement other command list types
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
		default:
			return _mGraphicsCommandQueue.GetCommandQueue();
		}
	}
}
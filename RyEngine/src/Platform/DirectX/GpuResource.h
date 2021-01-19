#pragma once

#include "pch.h"

using Microsoft::WRL::ComPtr;

namespace RE
{
	class GpuResource
	{
	public:
		GpuResource() :
			_mUsageStates(D3D12_RESOURCE_STATE_COMMON),
			_mTransitioningState((D3D12_RESOURCE_STATES)-1),
			_mGpuVirtualAddress((D3D12_GPU_VIRTUAL_ADDRESS)0)
		{}

		GpuResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES currentState) :
			_mResource(resource),
			_mUsageStates(currentState),
			_mTransitioningState((D3D12_RESOURCE_STATES)-1),
			_mGpuVirtualAddress((D3D12_GPU_VIRTUAL_ADDRESS)0)
		{}

		virtual void Destroy()
		{
			_mResource = nullptr;
			_mGpuVirtualAddress = (D3D12_GPU_VIRTUAL_ADDRESS)0;
		}

		ID3D12Resource* operator->() { return _mResource; }
		const ID3D12Resource* operator->() const { return _mResource; }

		ID3D12Resource* GetResource() { return _mResource; }
		const ID3D12Resource* GetResource() const { return _mResource; }

		const D3D12_RESOURCE_STATES GetCurrentState() const { return _mUsageStates; }
		void SetCurrentState(D3D12_RESOURCE_STATES state) { _mUsageStates = state; }

	protected:
		ID3D12Resource* _mResource;
		D3D12_RESOURCE_STATES _mUsageStates;
		D3D12_RESOURCE_STATES _mTransitioningState;
		D3D12_GPU_VIRTUAL_ADDRESS _mGpuVirtualAddress;
	};
}
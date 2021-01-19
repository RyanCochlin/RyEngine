#include "pch.h"
#include "DirectxAPI.h"
#include "ColorBuffer.h"
#include "Core/SubSystemManager.h"

namespace RE
{
	void ColorBuffer::CreateFromSwapChain(ID3D12Device* device, ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE handle)
	{
		AttachResource(resource, D3D12_RESOURCE_STATE_PRESENT);

		_mRtvDescriptorHandle = handle;
		device->CreateRenderTargetView(_mResource, nullptr, _mRtvDescriptorHandle);
	}

	void ColorBuffer::SetClearColor(Color color)
	{
		_mClearColor = color;
	}

	Color ColorBuffer::GetClearColor()
	{
		return _mClearColor;
	}
}
#include "pch.h"
#include "DirectxAPI.h"
#include "ColorBuffer.h"
#include "Core/SubSystemManager.h"

namespace RE
{
	void ColorBuffer::CreateFromSwapChain(ID3D12Resource* resource)
	{
		AttachResource(resource, D3D12_RESOURCE_STATE_PRESENT);

		RyDirectX* api = SubSystemManager::Instance().GFX()->GraphicsAPI();
		_mRtvDescriptorHandle = api->AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		api->_mDevice->CreateRenderTargetView(_mResource.Get(), nullptr, _mRtvDescriptorHandle);
	}
}
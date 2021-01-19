#include "pch.h"
#include "PixelBuffer.h"

namespace RE
{
	void PixelBuffer::AttachResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES currentState)
	{
		D3D12_RESOURCE_DESC desc = resource->GetDesc();

		_mResource = resource;
		_mUsageStates = currentState;

		_mWidth = desc.Width;
		_mHeight = desc.Height;
		_mArraySize = desc.DepthOrArraySize;
		_mFormat = desc.Format;
	}
}
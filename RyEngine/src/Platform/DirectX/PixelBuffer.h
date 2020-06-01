#pragma once
#include "GpuResource.h"

namespace RE
{
	class PixelBuffer : public GpuResource
	{
	public:
		PixelBuffer() :
			_mWidth(0),
			_mHeight(0),
			_mArraySize(0),
			_mFormat(DXGI_FORMAT_UNKNOWN)
		{}

		uint32_t Width() const { return _mWidth; }
		uint32_t Height() const { return _mHeight; }

	protected:
		void AttachResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES currentState);

		uint32_t _mWidth;
		uint32_t _mHeight;
		uint32_t _mArraySize;
		DXGI_FORMAT _mFormat;
	};
}

#pragma once
#include "PixelBuffer.h"
#include "Core/Color.h"

namespace RE
{
	class ColorBuffer : public PixelBuffer
	{
	public:
		ColorBuffer(Color clearColor = Color{ 0.0f, 0.0f, 0.0f, 1.0f }) :
			_mClearColor(clearColor),
			_mRtvDescriptorHandle()
		{}

		void CreateFromSwapChain(ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE handle);

		void SetClearColor(Color color);
		Color GetClearColor();

		const D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() { return _mRtvDescriptorHandle; }

	protected:
		Color _mClearColor;
		D3D12_CPU_DESCRIPTOR_HANDLE _mRtvDescriptorHandle;
	};
}

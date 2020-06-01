#pragma once

#include "RyDirectX.h"

namespace RE
{
	RyDirectX _gDirectX;

	class DirectxAPI
	{
		friend class ColorBuffer;
	public:
		void Init()
		{
			_gDirectX.Init();
		}

		void Release() { _gDirectX.Release(); }
		void OnRender() { _gDirectX.OnRender(); }

	private:
		static RyDirectX DX() { return _gDirectX; }
		static ID3D12Device* Device() { return DX()._mDevice.Get(); }
		static D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type) { return DX().AllocateDescriptor(type); }
	};
}
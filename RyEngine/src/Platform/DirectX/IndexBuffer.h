#pragma once
#include "GpuResource.h"

namespace RE
{
	class MeshGeometry;

	class IndexBuffer : public GpuResource
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		UINT GetSize() { return _mCount * _mIndexStride; }
		UINT GetCount() { return _mCount; }
		UINT GetStride() { return _mIndexStride; }

		void Create(ID3D12Device* device, UINT numElements, UINT elementSize);
		D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress();

	private:
		UINT _mIndexStride;
		UINT _mCount;
	};
}
#pragma once
#include "GpuResource.h"
#include "Core/Vertex.h"

namespace RE
{
	class MeshGeometry;

	class VertexBuffer : public GpuResource
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		UINT GetSize() { return _mCount * _mVertStride; }
		UINT GetCount() { return _mCount; }
		UINT GetStride() { return _mVertStride; }

		void Create(ID3D12Device* device, UINT numElements, UINT elementSize);
		D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress();

	private:
		Vertex* _mVertex;
		UINT _mVertStride;
		UINT _mCount;
	};
}


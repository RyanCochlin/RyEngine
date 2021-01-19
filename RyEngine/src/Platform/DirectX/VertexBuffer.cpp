#include "pch.h"
#include "VertexBuffer.h"
#include "Core/Vertex.h"
#include "MeshGeometry.h"

namespace RE
{
	VertexBuffer::VertexBuffer() : 
		_mVertStride(sizeof(Vertex)),
		_mCount(0),
		_mVertex(nullptr)
	{}

	VertexBuffer::~VertexBuffer()
	{
		delete _mVertex;
	}

	void VertexBuffer::Create(ID3D12Device* device, UINT numElements, UINT elementSize)
	{
		UINT byteSize = numElements * elementSize;

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&_mResource)));
	}

	D3D12_GPU_VIRTUAL_ADDRESS VertexBuffer::GetGpuAddress()
	{
		return _mResource->GetGPUVirtualAddress();
	}
}
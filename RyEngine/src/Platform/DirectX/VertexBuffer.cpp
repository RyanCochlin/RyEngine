#include "pch.h"
#include "VertexBuffer.h"
#include "Core/Vertex.h"
#include "MeshGeometry.h"
#include "DirectXCore.h"

namespace RE
{
	VertexBuffer::VertexBuffer(std::vector<Vertex>& verts)
	{
		_mVerticies = verts;
		_mVertStride = sizeof(Vertex);
		_mCount = _mVerticies.size();
		Create();
	}

	VertexBuffer::VertexBuffer(std::vector<Vertex>&& verts)
	{
		_mVerticies = verts;
		_mVertStride = sizeof(Vertex);
		_mCount = _mVerticies.size();
		Create();
	}

	void VertexBuffer::Create()
	{
		UINT byteSize = _mVertStride * _mCount;

		ThrowIfFailed(DirectXCore::GetDevice()->CreateCommittedResource(
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
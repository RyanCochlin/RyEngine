#pragma once
#include "GpuResource.h"
#include "Core/Vertex.h"

namespace RE
{
	class MeshGeometry;

	class VertexBuffer : public GpuResource
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(VertexBuffer&&) = default;
		VertexBuffer(std::vector<Vertex>& verts);
		VertexBuffer(std::vector<Vertex>&& verts);
		~VertexBuffer() = default;

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		UINT GetSize() { return _mCount * _mVertStride; }
		UINT GetCount() { return _mCount; }
		UINT GetStride() { return _mVertStride; }
		const std::vector<Vertex>& GetVerticies() const { return _mVerticies; }

		void Create();
		D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress();

	private:
		std::vector<Vertex> _mVerticies;
		UINT _mVertStride;
		UINT _mCount;
	};
}


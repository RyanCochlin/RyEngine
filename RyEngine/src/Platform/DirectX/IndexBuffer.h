#pragma once
#include "GpuResource.h"
#include "Core/Math/CoreMath.h"

namespace RE
{
	class MeshGeometry;

	class IndexBuffer : public GpuResource
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(IndexBuffer&&) = default;
		IndexBuffer(std::vector<RE_INDEX>& indicies);
		IndexBuffer(std::vector<RE_INDEX>&& indicies);

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		~IndexBuffer() = default;

		UINT GetSize() { return _mCount * _mIndexStride; }
		UINT GetCount() { return _mCount; }
		UINT GetStride() { return _mIndexStride; }
		const std::vector<RE_INDEX>& GetIndicies() const { return _mIndicies; }

		void Create();
		D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress();

	private:
		std::vector<RE_INDEX> _mIndicies;
		UINT _mIndexStride;
		UINT _mCount;
	};
}
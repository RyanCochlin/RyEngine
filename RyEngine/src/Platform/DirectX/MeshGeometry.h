#pragma once

#include "Core/Vertex.h"
#include "Core/Graphics/Mesh.h"
#include "Core/Graphics/MeshManager.h"
#include "Core/Math/CoreMath.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UploadBuffer.h"

#include <vector>

namespace RE
{
	class VertexBuffer;
	class IndexBuffer;
	class UploadBuffer;

	class MeshGeometry
	{
		friend class GeometeryManager;

	public:
		MeshGeometry();
		MeshGeometry(VertexBuffer& vb, IndexBuffer& ib, std::vector<SubMeshData> subMeshData);
		MeshGeometry(MeshGeometry&&) = default;
		~MeshGeometry() = default;

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView();
		D3D12_INDEX_BUFFER_VIEW IndexBufferView();
		D3D12_SUBRESOURCE_DATA GetVertexResourceData();
		D3D12_SUBRESOURCE_DATA GetIndexResourceData();
		SubMeshData& GetSubMeshData(int index);

		inline size_t VertexCount() { return _mVertexBuffer.GetCount(); }
		inline size_t IndexCount() { return _mIndexBuffer.GetCount(); }
		inline UINT VertexElementSize() { return sizeof(Vertex); }
		inline UINT IndexElementSize() { return sizeof(RE_INDEX); }
		inline size_t VertexSize() { return VertexCount() * VertexElementSize(); }
		inline size_t IndexSize() { return IndexCount() * IndexElementSize(); }
		inline size_t SubMeshCount() { return _mSubMeshData.size(); }

	private:
		VertexBuffer _mVertexBuffer;
		IndexBuffer _mIndexBuffer;
		UploadBuffer _mVertexUploadResource;
		UploadBuffer _mIndexUploadResource;
		std::vector<SubMeshData> _mSubMeshData;
	};

	class GeometeryManager
	{
	public:
		GeometeryManager();
		~GeometeryManager();

		void UploadAll(ID3D12GraphicsCommandList* commandList);
		UINT MeshCount();
		UINT VertexCount();
		UINT IndexCount();
		void AddMesh(MeshGeometry& mesh);
		MeshGeometry& GetMesh(int index);

		bool IsDirty() const { return _mDirty; }

	private:
		std::vector<MeshGeometry> _mMeshes;
		bool _mDirty;
	};
}

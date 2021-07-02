#pragma once

#include "Core/Vertex.h"
#include "Core/Mesh.h"
#include "Core/Math/CoreMath.h"

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
		MeshGeometry(Mesh mesh);
		~MeshGeometry();

		Vertex& operator[](int index);
		Vertex* GetData();
		D3D12_VERTEX_BUFFER_VIEW VertexBufferView();
		D3D12_INDEX_BUFFER_VIEW IndexBufferView();
		D3D12_SUBRESOURCE_DATA GetResourceData();
		D3D12_SUBRESOURCE_DATA GetIndexResourceData();

		inline size_t VertexCount() { return _mVerticies.size(); }
		inline size_t IndexCount() { return _mIndicies.size(); }
		inline UINT ElementSize() { return sizeof(Vertex); }
		inline UINT IndexElementSize() { return sizeof(RE_INDEX); }
		inline size_t VertexSize() { return VertexCount() * ElementSize(); }
		inline size_t IndexSize() { return IndexCount() * IndexElementSize(); }

	private:
		std::vector<Vertex> _mVerticies;
		std::vector<RE_INDEX> _mIndicies;
		VertexBuffer* _mGpuResource;
		IndexBuffer* _mIndexBuffer;
		UploadBuffer* _mVertexUploadResource;
		UploadBuffer* _mIndexUploadResource;
		bool _mUploaded; //TODO figure out a better way to do this
	};

	class GeometeryManager
	{
	public:
		GeometeryManager();
		~GeometeryManager();

		void Submit(Mesh mesh);
		void UploadAll(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
		UINT MeshCount();
		UINT VertexCount();
		UINT IndexCount();
		MeshGeometry* GetMesh(int index);

	private:
		std::vector<MeshGeometry*> _mMeshes;
	};
}

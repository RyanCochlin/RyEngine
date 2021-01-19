#pragma once

#include "Core/Vertex.h"
#include "Core/Mesh.h"

#include <vector>

namespace RE
{
	class VertexBuffer;
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
		D3D12_VERTEX_BUFFER_VIEW MeshGeometry::VertexBufferView();
		D3D12_SUBRESOURCE_DATA GetResourceData();

		size_t Count() { return _mVerticies.size(); }
		UINT ElementSize() { return sizeof(Vertex); }
		size_t Size() { return Count() * ElementSize(); }

	private:
		std::vector<Vertex> _mVerticies;
		VertexBuffer* _mGpuResource;
		UploadBuffer* _mUploadResource;
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
		MeshGeometry* GetMesh(int index);

	private:
		std::vector<MeshGeometry*> _mMeshes;
	};
}

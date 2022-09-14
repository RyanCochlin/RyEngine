#pragma once

#include <unordered_map>
#include <memory>
#include <deque>
#include <utility>

#include "Core/Utils.h"
#include "Core/Singleton.h"
#include "Mesh.h"
#include "Material.h"

namespace RE
{
	using VertexPoolContainer = std::vector<std::shared_ptr<Vertex>>;
	using VertexPoolQueue = std::deque<std::shared_ptr<Vertex>>;

	class VertexPool
	{
	public:
		VertexPool();

		VertexPoolContainer Reserve(size_t count);
		void Free(VertexPoolContainer pool);

	private:
		void Allocate(const size_t count);

		VertexPoolContainer _mVertexPool;
		VertexPoolQueue _mFreeVertexPool;
	};

	struct SubMeshData
	{
		SubMeshData() = default;
		SubMeshData(Transform* t, Material* m, uint32_t bv, uint32_t bi, uint32_t ic) : transform(t), material(m), baseVertex(bv), baseIndex(bi), indexCount(ic) {}
		Transform* transform;
		Material* material;
		uint32_t baseVertex;
		uint32_t baseIndex;
		uint32_t indexCount;
	};

	//TODO use this class to push mesh data to GraphicsAPI
	class MeshHeapData
	{
	public:
		std::vector<Vertex> vertexHeap;
		std::vector<RE_INDEX> indexHeap;
		std::vector<SubMeshData> subMeshData;

		void Clear()
		{
			vertexHeap.clear();
			indexHeap.clear();
			subMeshData.clear();
		}

		bool IsEmpty()
		{
			return vertexHeap.size() <= 0 && indexHeap.size() <= 0;
		}
	};

	struct MeshInstanceData
	{
		MeshInstanceData(Mesh* m, Transform* t, Material* mat) : mesh(m)
		{
			instanceTransforms.push_back(std::move(t));
			instanceMaterials.push_back(mat);
		}
		Mesh* mesh;
		std::vector<Transform*> instanceTransforms;
		std::vector<Material*> instanceMaterials;
	};

	class MeshManager
	{
	public:
		MeshManager();
		MeshManager(const MeshManager& m) = delete;
		MeshManager& operator=(const MeshManager& m) = delete;

		const std::vector<MeshInstanceData>& GetMeshData() const { return _mInstancedMeshes; }
		void OnUpdate();
		void AddMesh(Mesh* mesh, Transform* trans, Material* mat);
		void AddInstance(const char* id, Transform* trans, Material* mat);
		Mesh* GetInstance(const char* id);
		bool FlushMeshData(MeshHeapData& meshData);

	private:
		VertexPool _mVertPool;
		std::vector<Mesh> _mMeshHeap; // TODO delete this one
		std::vector<MeshInstanceData> _mInstancedMeshes;
		MeshHeapData _mMeshHeapData;
		uint32_t _mTotalVerts;
		uint32_t _mTotalIndicis;
		bool _mDirty;
	};
}

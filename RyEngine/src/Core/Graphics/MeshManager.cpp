#include "pch.h"
#include <cstring>
#include <algorithm>
#include "MeshManager.h"
#include "IGraphicsAPI.h"

namespace RE
{
	//-----------------------------Vertex Pool-----------------------------//
	VertexPool::VertexPool()
	{
		_mVertexPool.reserve(100);

		for (auto& vertex : _mVertexPool)
		{
			_mFreeVertexPool.push_back(vertex);
		}
	}

	VertexPoolContainer VertexPool::Reserve(size_t count)
	{
		// determine if we need to allocate more
		size_t poolSize = _mVertexPool.size();
		size_t freeCount = _mFreeVertexPool.size();
		if (freeCount < count)
		{
			Allocate(count - freeCount);
		}

		VertexPoolContainer ret;
		for (int i = 0; i < count; i++)
		{
			std::shared_ptr<Vertex> vert = _mFreeVertexPool[i];
			ret.push_back(vert);
			_mFreeVertexPool.pop_front();
		}

		return ret;
	}

	void VertexPool::Free(VertexPoolContainer pool)
	{
		for (auto& vertex : pool)
		{
			_mFreeVertexPool.push_back(vertex);
		}

		pool.clear();
	}

	void VertexPool::Allocate(const size_t count)
	{
		size_t endIndex = _mVertexPool.size();
		_mVertexPool.insert(_mVertexPool.end(), count, {});
		for (int i = endIndex; i < endIndex + count; i++)
		{
			auto& vertex = _mVertexPool[i];
			_mFreeVertexPool.push_back(vertex);
		}
	}

	//-----------------------------Mesh Manager-----------------------------//
	MeshManager::MeshManager() :
		_mVertPool{},
		_mMeshHeapData{},
		_mTotalVerts(0),
		_mTotalIndicis(0),
		_mDirty(false)
	{}

	void MeshManager::OnUpdate()
	{
		if (_mDirty)
		{
			// build MeshHeapData
			_mMeshHeapData.vertexHeap.reserve(_mTotalVerts);
			_mMeshHeapData.indexHeap.reserve(_mTotalIndicis);
			uint32_t baseVertex = 0;
			uint32_t baseIndex = 0;
			uint32_t indexCount = 0;

			for (auto& meshInstance : _mInstancedMeshes)
			{
				Mesh* mesh = meshInstance.mesh;
				MeshData& meshData = mesh->GetMeshData();
				indexCount = meshData.indicies.size();

				_mMeshHeapData.vertexHeap.insert(_mMeshHeapData.vertexHeap.end(), meshData.verticies.begin(), meshData.verticies.end());
				_mMeshHeapData.indexHeap.insert(_mMeshHeapData.indexHeap.end(), meshData.indicies.begin(), meshData.indicies.end());
				for (auto transform : meshInstance.instanceTransforms)
				{
					_mMeshHeapData.subMeshData.emplace_back(transform, baseVertex, baseIndex, indexCount);
				}

				baseVertex += meshData.verticies.size();
				baseIndex += meshData.indicies.size();
			}

			_mDirty = false;
		}
	}

	void MeshManager::AddMesh(Mesh* mesh, Transform* trans)
	{
		MeshInstanceData instance( mesh, trans );
		_mInstancedMeshes.push_back(instance);
		_mTotalVerts += mesh->GetVerticies().size();
		_mTotalIndicis += mesh->GetIndicies().size();
		_mDirty = true;
	}

	void MeshManager::AddInstance(const char* id, Transform* trans)
	{
		for (auto& instance : _mInstancedMeshes)
		{
			if (strcmp(instance.mesh->GetID(), id) == 0)
			{
				instance.instanceTransforms.push_back(trans);
				break;
			}
		}
	}

	Mesh* MeshManager::GetInstance(const char* id)
	{
		auto& item = std::find_if(_mInstancedMeshes.begin(), _mInstancedMeshes.end(), [&](MeshInstanceData inst) {
			return inst.mesh->GetID() == id;
		});

		return item != _mInstancedMeshes.end() ? item->mesh : nullptr;
	}

	bool MeshManager::FlushMeshData(MeshHeapData& meshData)
	{
		if (!_mMeshHeapData.IsEmpty())
		{
			meshData = std::move(_mMeshHeapData);
			_mMeshHeapData.Clear();
			_mTotalVerts = 0;
			_mTotalIndicis = 0;
			return true;
		}

		return false;
	}
}
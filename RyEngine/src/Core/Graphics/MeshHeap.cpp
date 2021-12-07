#include "pch.h"
#include "MeshHeap.h"
#include "Mesh.h"
#include <algorithm>


namespace RE
{
	MeshHeap::~MeshHeap()
	{
		_mMeshHeap.clear();
	}

	void MeshHeap::PushMesh(Mesh mesh)
	{
		_mMeshHeap.push_back(mesh);
	}

	bool MeshHeap::ConsumeMesh(Mesh& mesh)
	{
		if(_mMeshHeap.size() > 0)
		{
			mesh = std::move(_mMeshHeap[0]);
			_mMeshHeap.pop_front();
			return true;
		}
		return false;
	}
}
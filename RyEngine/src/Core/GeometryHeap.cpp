#include "pch.h"
#include "GeometryHeap.h"
#include "Mesh.h"
#include <algorithm>


namespace RE
{
	GeometryHeap::GeometryHeap() :
		_mMeshHeap{}
	{

	}

	void GeometryHeap::PushMesh(Mesh* mesh)
	{
		_mMeshHeap.push_back(mesh);
	}

	void GeometryHeap::RemoveMesh(Mesh* mesh)
	{
		std::vector<Mesh*>::iterator i = std::find(_mMeshHeap.begin(), _mMeshHeap.end(), mesh);
		if (i != _mMeshHeap.end())
		{
			_mMeshHeap.erase(i);
		}
	}
}
#pragma once

#include "Mesh.h"

#include <vector>

namespace RE
{
	class GeometryHeap
	{
	public:
		GeometryHeap();

		void PushMesh(Mesh* mesh);
		void RemoveMesh(Mesh* mesh);

		const std::vector<Mesh*> GetHeap() { return _mMeshHeap; }

	private:
		std::vector<Mesh*> _mMeshHeap;
	};
}
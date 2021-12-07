#pragma once

#include "Mesh.h"

#include <deque>

namespace RE
{
	class MeshHeap
	{
	public:
		MeshHeap() = default;
		~MeshHeap();
		MeshHeap(const MeshHeap&) = delete;
		MeshHeap& operator=(const MeshHeap&) = delete;

		void PushMesh(Mesh mesh);
		bool ConsumeMesh(Mesh& mesh);

		//const std::vector<Mesh> GetHeap() { return _mMeshHeap; }

	private:
		std::deque<Mesh> _mMeshHeap;
	};
}
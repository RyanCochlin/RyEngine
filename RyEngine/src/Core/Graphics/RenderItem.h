#pragma once

#include <vector>
#include "Core/Math/Matrix.h"
#include "Mesh.h"

namespace RE
{
	class RenderItem
	{
	public:
		RenderItem() = default;

		const Matrix4& GetWorld() const;
		void SetWorld(const Matrix4& world);
		void AddSubmesh(SubMesh sm, std::vector<Vertex> verts);

	private:
		Mesh _mMesh;
		Matrix4 _mWorld;
	};
}

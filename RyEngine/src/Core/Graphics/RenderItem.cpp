#include "pch.h"
#include "RenderItem.h"

namespace RE
{
	const Matrix4& RenderItem::GetWorld() const
	{
		return _mWorld;
	}

	void RenderItem::SetWorld(const Matrix4& world)
	{
		_mWorld = world;
		//bongo4 = bongo1 + bongo2;
	}

	void RenderItem::AddSubmesh(SubMesh sm, std::vector<Vertex> verts)
	{
		// HULLOOOO - Fluegle was here
	}
}

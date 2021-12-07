#pragma once

#include<memory>

#include "Core/GlobalDefines.h"
#include "Core/SubSystemManager.h"
#include "Mesh.h"


namespace RE::GeometryGenerator
{
	RE_API Triangle GetTriangle(Vector3& p1, Vector3& p2, Vector3& p3);
	RE_API Square GetSquare(uint32_t width, uint32_t height, Vector3 center);
	RE_API Cube GetCube(uint32_t size, Vector3 center);

	// TODO maybe move these to their own cpp file?
	Triangle GetTriangle(Vector3& p1, Vector3& p2, Vector3& p3)
	{
		//TODO fetch verts from vertex pool
		Triangle t(p1, p2, p3);
		Transform trans;
		SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->AddMesh(t, trans);
		return t;
	}

	Square GetSquare(uint32_t width, uint32_t height, Vector3 center)
	{
		Square s(width, height, center);
		Transform trans;
		SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->AddMesh(s, trans);
		return s;
	}

	Cube GetCube(uint32_t size, Vector3 center)
	{
		Cube c(size, center);
		Transform trans;
		SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->AddMesh(c, trans);
		return c;
	}
}
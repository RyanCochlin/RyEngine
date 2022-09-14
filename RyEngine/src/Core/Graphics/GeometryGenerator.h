#pragma once

#include<memory>

#include "Core/GlobalDefines.h"
#include "Core/SubSystemManager.h"
#include "Mesh.h"
#include "Core/ECS/ECS.h"

namespace RE::GeometryGenerator
{
	RE_API Entity& GetTriangle(Vector3& p1, Vector3& p2, Vector3& p3);
	RE_API Entity& GetSquare(uint32_t width, uint32_t height, Vector3 center);
	RE_API Entity& GetCube(uint32_t size, Vector3 center);

	// TODO maybe move these to their own cpp file?
	Entity& GetTriangle(Vector3& p1, Vector3& p2, Vector3& p3)
	{
		//TODO fetch verts from vertex pool
		Triangle* t = new Triangle(p1, p2, p3);
		Transform* trans = new Transform();
		Material* mat = SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->GetDefaultMaterial("TestName");
		SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->AddMesh(t, trans, mat);
		
		Entity entity;
		entity.mesh = t;
		entity.transform = trans;
		return entity;
	}

	Entity& GetSquare(uint32_t width, uint32_t height, Vector3 center)
	{
		Square* s = new Square(width, height, center);
		Transform* trans = new Transform();
		Material* mat = SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->GetDefaultMaterial("TestName");
		SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->AddMesh(s, trans, mat);
		
		Entity entity;
		entity.mesh = s;
		entity.transform = trans;
		return entity;
	}

	Entity& GetCube(uint32_t size, Vector3 center)
	{
		Transform* trans = new Transform();
		Material* mat = SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->GetDefaultMaterial("TestName");
		// TODO make this string a static member of the class to check against
		Mesh* c = SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->GetMeshInstance("cube");
		if (c == nullptr)
		{
			Cube* c = new Cube(size, center);
			SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->AddMesh(c, trans, mat);
		}
		else
		{
			SubSystemManager::Instance().GetSubSystem<GraphicsSystem>()->AddMeshInstance("cube", trans, mat);
		}

		// TODO this is temporary
		Entity entity;
		entity.mesh = c;
		entity.transform = trans;
		entity.material = mat;
		return entity;
	}
}
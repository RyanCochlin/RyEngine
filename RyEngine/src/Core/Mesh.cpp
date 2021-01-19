#include "pch.h"
#include "Mesh.h"
#include "SubSystemManager.h"
#include "Color.h"

#include <vector>

namespace RE
{
	Mesh::Mesh() :
		_mVerticies()
	{}

	Mesh::Mesh(std::vector<Vector3> vertList) :
		_mVerticies(vertList),
		_mColor(RE_WHITE)
	{}

	uint32_t Mesh::GetSize()
	{
		return _mVerticies.size() * sizeof(Vector3);
	}

	std::vector<Vector3> Mesh::GetVerticies()
	{
		return _mVerticies;
	}

	Vector3& Mesh::operator[](int index)
	{
		return _mVerticies[index];
	}

	void Mesh::SetColor(Color color)
	{
		_mColor = color;
	}

	Color Mesh::GetColor()
	{
		return _mColor;
	}

	void Mesh::AddVertex(Vector3 vert)
	{
		_mVerticies.push_back(vert);
	}

	void Mesh::Draw()
	{
		SubSystemManager::Instance().GFX()->AddMeshForDraw(this);
	}
}
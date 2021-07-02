#include "pch.h"
#include "Mesh.h"
#include "SubSystemManager.h"
#include "Color.h"
#include "Math/CoreMath.h"

#include <vector>

namespace RE
{
	Mesh::Mesh() :
		_mVerticies(),
		_mIndices(),
		_mSubMeshGeo(),
		_mColor(RE_WHITE)
	{}

	Mesh::Mesh(std::vector<Vertex> vertList, std::vector<RE_INDEX> indexList) :
		_mVerticies(vertList),
		_mIndices(indexList),
		_mSubMeshGeo(),
		_mColor(RE_WHITE)
	{
		SubMesh sub;
		sub.startIndex = 0;
		sub.baseVertex = 0;
		sub.indexCount = _mIndices.size();
		_mSubMeshGeo.push_back(sub);
	}

	uint32_t Mesh::GetSize()
	{
		return _mVerticies.size() * sizeof(Vector3);
	}

	std::vector<Vertex> Mesh::GetVerticies()
	{
		return _mVerticies;
	}

	std::vector<RE_INDEX> Mesh::GetIndicies()
	{
		return _mIndices;
	}

	Vertex& Mesh::operator[](int index)
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

	void Mesh::AddVertex(Vector3 pos, Color color /*= { -1.0f }*/)
	{
		Color c = (color.rgba[0] == -1.0f) ? _mColor : color;
		_mVerticies.push_back({ pos, ColorToVector(c) });
	}

	void Mesh::AddIndex(RE_INDEX ind)
	{
		_mIndices.push_back(ind);
	}

	void Mesh::AddVertAndIndex(Vector3 pos, RE_INDEX ind, Color color /*= { -1.0f }*/)
	{
		Color c = (color.rgba[0] == -1.0f) ? _mColor : color;
		_mVerticies.push_back({ pos, ColorToVector(c) });
		_mIndices.push_back(ind);
	}

	void Mesh::SetSubmesh(UINT indexCount)
	{
		SubMesh sub;
		UINT startInd = _mSubMeshGeo.back().startIndex + 1;
		UINT baseVert = _mSubMeshGeo.back().baseVertex + 1;
		_mSubMeshGeo.push_back(sub);
	}

	void Mesh::Draw()
	{
		SubSystemManager::Instance().GFX()->AddMeshForDraw(this);
	}
}
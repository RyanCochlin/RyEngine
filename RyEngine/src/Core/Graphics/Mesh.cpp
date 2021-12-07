#include "pch.h"
#include "Mesh.h"
#include "Core/SubSystemManager.h"
#include "Core/Color.h"
#include "Core/Math/CoreMath.h"

#include <vector>
#include <numeric>

namespace RE
{
	Mesh::Mesh(const char* id) :
		_mId(id),
		_mVerticies(),
		_mIndices(),
		_mSubMeshes(),
		_mColor(RE_WHITE)
	{}

	const char* Mesh::GetID()
	{
		return _mId;
	}

	uint32_t Mesh::VertexCount()
	{
		return _mMeshData.verticies.size();
	}

	uint32_t Mesh::IndexCount()
	{
		return _mMeshData.indicies.size();
	}

	void Mesh::AddMeshData(const MeshData& data)
	{
		uint32_t subMeshVertIndex = VertexCount();
		uint32_t subMeshIndIndex = IndexCount();
		_mSubMeshes.emplace_back(data.indicies.size(), subMeshIndIndex, subMeshVertIndex);
		_mMeshData = data;
	}

	MeshData& Mesh::GetMeshData()
	{
		return _mMeshData;
	}

	uint32_t Mesh::GetSize()
	{
		return _mMeshData.verticies.size() * sizeof(Vector3);
	}

	std::vector<Vertex>& Mesh::GetVerticies()
	{
		return _mMeshData.verticies;
	}

	std::vector<RE_INDEX> Mesh::GetIndicies()
	{
		return _mMeshData.indicies;
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

	void Mesh::SetSubmesh(uint32_t indexCount)
	{
		SubMesh sub;
		UINT startInd = _mSubMeshes.back().startIndex + 1;
		UINT baseVert = _mSubMeshes.back().baseVertex + 1;
		_mSubMeshes.push_back(sub);
	}

	void Mesh::AddSubmesh(uint32_t indexCount, std::vector<Vertex>& mesh)
	{
		_mVerticies.insert(_mVerticies.end(), mesh.begin(), mesh.end());
		SetSubmesh(indexCount);
	}
}
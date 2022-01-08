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

	void Mesh::SetColor(Color color)
	{
		_mColor = color;
	}

	Color Mesh::GetColor()
	{
		return _mColor;
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
		SetSubmesh(indexCount);
	}
}
#pragma once
#include <vector>

#include "Transform.h"
#include "Core/Vertex.h"
#include "Core/Math/Vector.h"
#include "Core/Math/CoreMath.h"
#include "Core/Color.h"

namespace RE
{
	enum class BasicGeometryType
	{
		TRIANGLE,
		SQUARE,
		CUBE,
	};

	//-------------------------SubMesh--------------------------------//
	struct SubMesh
	{
		// TODO SubMesh might no longer be the correct term here. Going to keep it for now
		SubMesh() = default;
		SubMesh(uint32_t ic, uint32_t si, uint32_t bv) :
			indexCount(ic),
			startIndex(si),
			baseVertex(bv)
		{}

		uint32_t indexCount;
		uint32_t startIndex;
		uint32_t baseVertex;
	};

	struct MeshData
	{
		std::vector<Vertex> verticies;
		std::vector<RE_INDEX> indicies;
	};

	//-------------------------Mesh--------------------------------//
	class RE_API Mesh //: public Component
	{
	public:
		Mesh(const char* id);

		Vertex& operator[](int index);
		
		uint32_t VertexCount();
		uint32_t IndexCount();
		std::vector<Vertex>& GetVerticies();
		std::vector<RE_INDEX> GetIndicies();
		void AddMeshData(const MeshData& data);
		MeshData& GetMeshData();

		const char* GetID();
		uint32_t GetSize();
		void SetColor(Color color);
		Color GetColor();
		void SetSubmesh(uint32_t indexCount);
		void AddSubmesh(uint32_t indexCount, std::vector<Vertex>& mesh);

	protected:
		MeshData _mMeshData;
		std::vector<SubMesh> _mSubMeshes;
		const char* _mId;
		Color _mColor;
	};

	//-------------------------Basic Geometry-------------------------//
	class BasicGeometry : public Mesh
	{
	public:
		BasicGeometry() = default;
		BasicGeometry(BasicGeometryType type, const char* id) : geometryType(type), Mesh(id)
		{}
		
		BasicGeometryType geometryType;
	};

	class RE_API Triangle : public BasicGeometry
	{
	public:
		Triangle() : BasicGeometry{ BasicGeometryType::TRIANGLE, "triangle" } {}
		Triangle(Vector3& p1, Vector3& p2, Vector3& p3) :
			BasicGeometry{ BasicGeometryType::TRIANGLE, "triangle" }
		{
			MeshData md;
			md.verticies.emplace_back(p1, ColorToVector(RE_WHITE));
			md.verticies.emplace_back(p2, ColorToVector(RE_WHITE));
			md.verticies.emplace_back(p3, ColorToVector(RE_WHITE));
			md.indicies.push_back(0);
			md.indicies.push_back(1);
			md.indicies.push_back(2);
			AddMeshData(md);
		}
	};

	class RE_API Square : public BasicGeometry
	{
	public:
		Square() : BasicGeometry{ BasicGeometryType::SQUARE, "square" } {}
		Square(UINT width, UINT height, Vector3 center) :
			BasicGeometry{ BasicGeometryType::SQUARE, "square" }
		{
			float halfWidth = width / 2.0f;
			float halfHeight = height / 2.0f;
			float fDepth = (float)center.z;

			MeshData md;
			md.verticies.emplace_back(Vector3{ halfWidth, halfHeight, fDepth }, ColorToVector(RE_WHITE));
			md.verticies.emplace_back(Vector3{ halfWidth, -halfHeight, fDepth }, ColorToVector(RE_WHITE));
			md.verticies.emplace_back(Vector3{ -halfWidth, -halfHeight, fDepth }, ColorToVector(RE_WHITE));
			md.verticies.emplace_back(Vector3{ -halfWidth, halfHeight, fDepth }, ColorToVector(RE_WHITE));

			md.indicies.push_back(0);
			md.indicies.push_back(1);
			md.indicies.push_back(2);
			md.indicies.push_back(0);
			md.indicies.push_back(2);
			md.indicies.push_back(3);
			AddMeshData(md);
		}
	};

	class RE_API Cube : public BasicGeometry
	{
	public:
		Cube() : BasicGeometry{ BasicGeometryType::CUBE, "cube" } {}
		Cube(UINT size, Vector3 center) :
			BasicGeometry{ BasicGeometryType::CUBE, "cube" }
		{
			MeshData md;
			float halfSize = size / 2.0f;
			float frontDepth = center.z - halfSize;
			float backDepth = center.z + halfSize;

			//front verticies
			md.verticies.emplace_back( Vertex({halfSize + center.x, halfSize + center.y, frontDepth }, RE_RED) );
			md.verticies.emplace_back( Vertex({halfSize + center.x, -halfSize + center.y, frontDepth }, RE_GREEN) );
			md.verticies.emplace_back( Vertex({ -halfSize + center.x, -halfSize + center.y, frontDepth }, RE_BLUE) );
			md.verticies.emplace_back( Vertex({ -halfSize + center.x, halfSize + center.y, frontDepth }, RE_YELLOW) );

			//back verticies
			md.verticies.emplace_back( Vertex({ halfSize + center.x, halfSize + center.y, backDepth }, RE_CYAN) );
			md.verticies.emplace_back( Vertex({ halfSize + center.x, -halfSize + center.y, backDepth }, RE_WHITE) );
			md.verticies.emplace_back( Vertex({ -halfSize + center.x, -halfSize + center.y, backDepth }, RE_BLACK) );
			md.verticies.emplace_back( Vertex({ -halfSize + center.x, halfSize + center.y, backDepth }, RE_MAGENTA) );

			//front face
			md.indicies.push_back(0);
			md.indicies.push_back(1);
			md.indicies.push_back(2);
			md.indicies.push_back(0);
			md.indicies.push_back(2);
			md.indicies.push_back(3);

			//back face
			md.indicies.push_back(4);
			md.indicies.push_back(6);
			md.indicies.push_back(5);
			md.indicies.push_back(4);
			md.indicies.push_back(7);
			md.indicies.push_back(6);

			//left face
			md.indicies.push_back(3);
			md.indicies.push_back(2);
			md.indicies.push_back(6);
			md.indicies.push_back(3);
			md.indicies.push_back(6);
			md.indicies.push_back(7);

			//right face
			md.indicies.push_back(0);
			md.indicies.push_back(5);
			md.indicies.push_back(1);
			md.indicies.push_back(0);
			md.indicies.push_back(4);
			md.indicies.push_back(5);

			//top face
			md.indicies.push_back(4);
			md.indicies.push_back(0);
			md.indicies.push_back(3);
			md.indicies.push_back(4);
			md.indicies.push_back(3);
			md.indicies.push_back(7);

			//bottom face
			md.indicies.push_back(1);
			md.indicies.push_back(5);
			md.indicies.push_back(2);
			md.indicies.push_back(2);
			md.indicies.push_back(5);
			md.indicies.push_back(6);

			AddMeshData(md);
		}
	};
}
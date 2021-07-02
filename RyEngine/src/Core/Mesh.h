#pragma once
#include <vector>

#include "Vertex.h"
#include "Math/Vector.h"
#include "Math/CoreMath.h"
#include "Color.h"

namespace RE
{
	//-------------------------SubMesh--------------------------------//
	struct SubMesh
	{
		UINT indexCount;
		UINT startIndex;
		UINT baseVertex;
	};

	class RE_API Mesh
	{
	public:
		Mesh();
		Mesh(std::vector<Vertex> vertList, std::vector<RE_INDEX> indexList);

		Vertex& operator[](int index);

		uint32_t GetSize();
		std::vector<Vertex> GetVerticies();
		std::vector<RE_INDEX> GetIndicies();
		void SetColor(Color color);
		Color GetColor();
		void AddVertex(Vector3 pos, Color color = { -1.0f });
		void AddIndex(RE_INDEX ind);
		void AddVertAndIndex(Vector3 pos, RE_INDEX ind, Color color = { -1.0f });
		void SetSubmesh(UINT indexCount);
		void Draw();

	protected:
		std::vector<SubMesh> _mSubMeshGeo;
		std::vector<Vertex> _mVerticies;
		std::vector<RE_INDEX> _mIndices;
		Color _mColor;
	};

	//-------------------------Basic Geometry-------------------------//
	class RE_API Triangle : public Mesh
	{
	public:
		Triangle(Vector3 p1, Vector3 p2, Vector3 p3)
		{
			AddVertAndIndex(p1, 0);
			AddVertAndIndex(p2, 1);
			AddVertAndIndex(p3, 2);
		}
	};

	class RE_API Square : public Mesh
	{
	public:
		Square(UINT width, UINT height, Vector3 center)
		{
			float halfWidth = width / 2.0f;
			float halfHeight = height / 2.0f;
			float fDepth = (float)center.z;

			AddVertex({halfWidth, halfHeight, fDepth});
			AddVertex({ halfWidth, -halfHeight, fDepth });
			AddVertex({-halfWidth, -halfHeight, fDepth});
			AddVertex({-halfWidth, halfHeight, fDepth});

			AddIndex(0);
			AddIndex(1);
			AddIndex(2);
			AddIndex(0);
			AddIndex(2);
			AddIndex(3);
		}
	};

	class RE_API Cube : public Mesh
	{
	public:
		Cube(UINT size, Vector3 center)
		{
			float halfSize = size / 2.0f;
			float frontDepth = center.z - halfSize;
			float backDepth = center.z + halfSize;

			//front verticies
			AddVertex({ halfSize + center.x, halfSize + center.y, frontDepth }, RE_RED);
			AddVertex({ halfSize + center.x, -halfSize + center.y, frontDepth }, RE_GREEN);
			AddVertex({ -halfSize + center.x, -halfSize + center.y, frontDepth }, RE_BLUE);
			AddVertex({ -halfSize + center.x, halfSize + center.y, frontDepth }, RE_YELLOW);

			//back verticies
			AddVertex({ halfSize + center.x, halfSize + center.y, backDepth }, RE_CYAN);
			AddVertex({ halfSize + center.x, -halfSize + center.y, backDepth }, RE_WHITE);
			AddVertex({ -halfSize + center.x, -halfSize + center.y, backDepth }, RE_BLACK);
			AddVertex({ -halfSize + center.x, halfSize + center.y, backDepth }, RE_MAGENTA);

			//front face
			AddIndex(0);
			AddIndex(1);
			AddIndex(2);
			AddIndex(0);
			AddIndex(2);
			AddIndex(3);

			//left face
			AddIndex(3);
			AddIndex(2);
			AddIndex(6);
			AddIndex(3);
			AddIndex(6);
			AddIndex(7);

			//right face
			AddIndex(0);
			AddIndex(5);
			AddIndex(1);
			AddIndex(0);
			AddIndex(4);
			AddIndex(5);

			//top face
			AddIndex(4);
			AddIndex(0);
			AddIndex(3);
			AddIndex(4);
			AddIndex(3);
			AddIndex(7);

			//bottom face
			AddIndex(1);
			AddIndex(5);
			AddIndex(2);
			AddIndex(2);
			AddIndex(5);
			AddIndex(6);
		}
	};
}
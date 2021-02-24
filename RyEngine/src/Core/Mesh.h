#pragma once
#include <vector>

#include "Vertex.h"
#include "Math/Vector.h"
#include "Color.h"

namespace RE
{
	class RE_API Mesh
	{
	public:
		Mesh();
		Mesh(std::vector<Vector3> vertList);

		Vector3& operator[](int index);

		uint32_t GetSize();
		std::vector<Vector3> GetVerticies();
		void SetColor(Color color);
		Color GetColor();
		void AddVertex(Vector3 vert);
		void Draw();

	protected:
		std::vector<Vector3> _mVerticies;
		Color _mColor;
	};

	//-------------------------Basic Geometry-------------------------//
	class RE_API Triangle : public Mesh
	{
	public:
		Triangle(Vector3 p1, Vector3 p2, Vector3 p3)
		{
			AddVertex(p1);
			AddVertex(p2);
			AddVertex(p3);
		}
	};
}
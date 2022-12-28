#pragma once

#include "Math/Vector.h"
#include "Color.h"

namespace RE
{
	struct Vertex
	{
		Vertex() = default;

		Vertex(Vector3 p, Color c) :
			Vertex(p, ColorToVector(c))
		{}

		Vertex(Vector3 p, Vector4 c) :
			position(p),
			normal(),
			color(c) {}

		Vector3 position;
		Vector3 normal;
		Vector4 color;
	};
}


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
			color(c) {}

		Vector3 position;
		Vector4 color;
	};
}


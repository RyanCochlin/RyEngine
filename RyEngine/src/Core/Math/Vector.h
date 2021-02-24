#pragma once

#include <math.h>

namespace RE
{
	struct Vector3
	{
		float x;
		float y;
		float z;

		inline float& operator[](int i)
		{
			ASSERT(i < 3);
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			}
		}
	};

	struct Vector4
	{
		float x;
		float y;
		float z;
		float w;

		inline float& operator[](int i)
		{
			ASSERT(i < 4);
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
			}
		}
	};

	inline Vector3 operator+(Vector3 left, Vector3 right)
	{
		return Vector3{ left.x + right.x, left.y + right.y, left.z + right.z };
	}

	inline Vector3 operator-(Vector3 left, Vector3 right)
	{
		return Vector3{ left.x - right.x, left.y - right.y, left.z - right.z };
	}

	inline Vector3 operator*(float s, Vector3 v)
	{
		return Vector3{ v.x * s, v.y * s, v.z * s };
	}

	inline float dot(Vector3 left, Vector3 right)
	{
		return	(left.x * right.x) +
				(left.y * right.y) +
				(left.z * right.z);
	}

	inline Vector3 cross(Vector3 left, Vector3 right)
	{
		return Vector3{ 
			(left.y * right.z) - (left.z * right.y),
			(left.z * right.x) - (left.x * right.z),
			(left.x * right.y) - (left.y * right.x)
		};
	}

	inline float sqrMagnitutde(Vector3 v)
	{
		return	(v.x * v.x) +
				(v.y * v.y) +
				(v.z * v.z);
	}

	inline float magnitutde(Vector3 v)
	{
		return sqrtf( sqrMagnitutde(v) );
	}

	inline Vector3 normalize(Vector3 v)
	{
		float m = magnitutde(v);
		return (1.0f / m) * v;
	}

	inline Vector4 operator+(Vector4 left, Vector4 right)
	{
		return Vector4{ left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w };
	}

	inline Vector4 operator-(Vector4 left, Vector4 right)
	{
		return Vector4{ left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w };
	}

	inline Vector4 operator*(float s, Vector4 v)
	{
		return Vector4{ v.x * s, v.y * s, v.z * s, v.w * s };
	}

	inline float dot(Vector4 left, Vector4 right)
	{
		return	(left.x * right.x) +
				(left.y * right.y) +
				(left.z * right.z) +
				(left.w * right.w);
	}

	inline float sqrMagnitutde(Vector4 v)
	{
		return	(v.x * v.x) +
				(v.y * v.y) +
				(v.z * v.z) +
				(v.w * v.w);
	}

	inline float magnitutde(Vector4 v)
	{
		return sqrtf( sqrMagnitutde(v) );
	}

	inline Vector4 normalize(Vector4 v)
	{
		float m = magnitutde(v);
		return (1.0f / m) * v;
	}
}
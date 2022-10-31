#pragma once

#include <math.h>

namespace RE
{
	struct Vector2
	{
		float x;
		float y;

		inline float& operator[](int i)
		{
			ASSERT(i < 2 && i >= 0);
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			}
		}
	};

	struct Vector3
	{
		float x;
		float y;
		float z;

		Vector3() = default;
		Vector3(float _x, float _y, float _z) :
			x(_x),
			y(_y),
			z(_z)
		{}

		inline float& operator[](int i)
		{
			ASSERT(i < 3 && i >= 0);
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

		Vector4() = default;
		Vector4(float _x, float _y, float _z, float _w) :
			x(_x),
			y(_y),
			z(_z),
			w(_w)
		{}

		inline float& operator[](int i)
		{
			ASSERT(i < 4 && i >= 0);
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
			}
		}
	};

	inline Vector2& operator+=(Vector2& left, Vector2& right)
	{
		left.x += right.x;
		left.y += right.y;
		return left;
	}

	inline Vector2 operator+(Vector2 left, Vector2 right)
	{
		return left += right;
	}

	inline Vector2& operator-=(Vector2& left, Vector2& right)
	{
		left.x -= right.x;
		left.y -= right.y;
		return left;
	}

	inline Vector2 operator-(Vector2 left, Vector2 right)
	{
		return Vector2{ left.x - right.x, left.y - right.y };
	}

	inline Vector2 operator*(float s, Vector2& v)
	{
		return Vector2{ v.x * s, v.y * s };
	}

	inline Vector2 operator/(float s, Vector2& v)
	{
		return Vector2{ v.x / s, v.y / s };
	}

	inline float dot(Vector2 left, Vector2 right)
	{
		return	(left.x * right.x) +
			(left.y * right.y);
	}

	inline float sqrMagnitutde(Vector2 v)
	{
		return	(v.x * v.x) +
			(v.y * v.y);
	}

	inline float magnitutde(Vector2 v)
	{
		return sqrtf(sqrMagnitutde(v));
	}

	inline Vector2 normalize(Vector2 v)
	{
		float m = magnitutde(v);
		return (1.0f / m) * v;
	}

	inline Vector3& operator+=(Vector3& left, Vector3& right)
	{
		left.x += right.x;
		left.y += right.y;
		left.z += right.z;
		return left;
	}

	inline Vector3 operator+(Vector3 left, Vector3 right)
	{
		return left += right;
	}

	inline Vector3& operator-=(Vector3& left, Vector3& right)
	{
		left.x -= right.x;
		left.y -= right.y;
		left.z -= right.z;
		return left;
	}

	inline Vector3 operator-(Vector3& left, Vector3& right)
	{
		return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
	}

	inline Vector3 operator*(float s, Vector3& v)
	{
		return Vector3{ v.x * s, v.y * s, v.z * s };
	}

	inline Vector3 operator/(float s, Vector3& v)
	{
		return Vector3{ v.x / s, v.y / s, v.z / s };
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

	inline float sqrMagnitutde(Vector3& v)
	{
		return	(v.x * v.x) +
				(v.y * v.y) +
				(v.z * v.z);
	}

	inline float magnitutde(Vector3& v)
	{
		return sqrtf( sqrMagnitutde(v) );
	}

	inline Vector3 normalize(Vector3& v)
	{
		float m = magnitutde(v);
		return (1.0f / m) * v;
	}

	inline Vector4& operator+=(Vector4& left, Vector4& right)
	{
		left.x += right.x;
		left.y += right.y;
		left.z += right.z;
		left.w += right.w;
		return left;
	}

	inline Vector4 operator+(Vector4 left, Vector4 right)
	{
		return left += right;
	}

	inline Vector4& operator-=(Vector4& left, Vector4& right)
	{
		left.x -= right.x;
		left.y -= right.y;
		left.z -= right.z;
		left.w -= right.w;
		return left;
	}

	inline Vector4 operator-(Vector4 left, Vector4 right)
	{
		return left -= right;
	}

	inline Vector4 operator*(float s, Vector4& v)
	{
		return Vector4{ v.x * s, v.y * s, v.z * s, v.w * s };
	}

	inline Vector4 operator/(float s, Vector4& v)
	{
		return Vector4{ v.x / s, v.y / s, v.z / s, v.w / s };
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
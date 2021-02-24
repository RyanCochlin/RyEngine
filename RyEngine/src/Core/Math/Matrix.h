#pragma once
#include "Vector.h"

namespace RE
{
	class Matrix3
	{
		friend Vector3 operator*(const Matrix3& m, const Vector3& v);
		friend Vector3 operator*(const Vector3& v, Matrix3& m);

	public:
		Matrix3()
		{
			//Identity
			_arr[0] = { 1.0f, 0.0f, 0.0f };
			_arr[1] = { 0.0f, 1.0f, 0.0f };
			_arr[2] = { 0.0f, 0.0f, 1.0f };
		}

		Matrix3(Vector3 x, Vector3 y, Vector3 z)
		{
			_arr[0] = x;
			_arr[1] = y;
			_arr[2] = z;
		}

		Vector3& operator[](UINT i);
		Vector3 row(UINT i);
		Vector3 col(UINT i);

		Matrix3 operator*(Matrix3& r);


	private:
		Vector3 _arr[3];
	};

	class Matrix4
	{
		friend Vector4 operator*(const Matrix4& m, const Vector4& v);
		friend Vector4 operator*(const Vector4& v, Matrix4& m);

	public:
		Matrix4()
		{
			//Identity
			_arr[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
			_arr[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
			_arr[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
			_arr[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		Matrix4(Vector4 x, Vector4 y, Vector4 z, Vector4 w)
		{
			_arr[0] = x;
			_arr[1] = y;
			_arr[2] = z;
			_arr[3] = w;
		}

		Vector4& operator[](UINT i);

		Vector4 row(UINT i);
		Vector4 col(UINT i);

		Matrix4 operator*(Matrix4& r);

	private:
		Vector4 _arr[4];
	};

	//---------------------------------------------------------------------//
	const Matrix3 I3;
	const Matrix4 I4;
}
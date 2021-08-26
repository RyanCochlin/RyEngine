#include "pch.h"
#include "Matrix.h"
#include "Vector.h"

namespace RE
{
	Vector3& Matrix3::operator[](UINT i)
	{
		ASSERT(i < 3);
		return _arr[i];
	}

	Vector3 Matrix3::row(UINT i)
	{
		ASSERT(i < 3);
		return _arr[i];
	}

	Vector3 Matrix3::col(UINT i)
	{
		ASSERT(i < 3);
		return {(*this)[0][i], (*this)[1][i], (*this)[2][i]};
	}

	Vector3 Matrix3::operator*(Vector3& v)
	{
		Vector3 r;
		r.x = dot(_arr[0], v);
		r.y = dot(_arr[1], v);
		r.z = dot(_arr[2], v);

		return r;
	}

	Vector3 operator*(Vector3& v, Matrix3& m)
	{
		Vector3 r;
		r.x = dot(v, m.col(0));
		r.y = dot(v, m.col(1));
		r.z = dot(v, m.col(2));

		return r;
	}

	Matrix3 Matrix3::operator*(Matrix3& other)
	{
		Matrix3 r;
		r[0][0] = dot(this->row(0), other.col(0));
		r[0][1] = dot(this->row(0), other.col(1));
		r[0][2] = dot(this->row(0), other.col(2));
		r[1][0] = dot(this->row(1), other.col(0));
		r[1][1] = dot(this->row(1), other.col(1));
		r[1][2] = dot(this->row(1), other.col(2));
		r[2][0] = dot(this->row(0), other.col(0));
		r[2][1] = dot(this->row(0), other.col(1));
		r[2][2] = dot(this->row(0), other.col(2));

		return r;
	}

	//---------------------------------------------------------------------------//
	Vector4& Matrix4::operator[](UINT i) 
	{
		ASSERT(i < 4);
		return _arr[i];
	}

	Vector4 Matrix4::row(UINT i)
	{
		ASSERT(i < 4);
		return _arr[i];
	}

	Vector4 Matrix4::col(UINT i)
	{
		ASSERT(i < 4);
		return { _arr[0][i], _arr[1][i], _arr[2][i], _arr[3][i] };
	}

	Vector4 operator*(Vector4& v, Matrix4& m)
	{
		Vector4 r;
		r.x = dot(v, m.col(0));
		r.y = dot(v, m.col(1));
		r.z = dot(v, m.col(2));
		r.w = dot(v, m.col(3));

		return r;
	}

	Vector4 Matrix4::operator*(Vector4& v)
	{
		Vector4 r;
		r.x = dot(_arr[0], v);
		r.y = dot(_arr[1], v);
		r.z = dot(_arr[2], v);
		r.w = dot(_arr[3], v);

		return r;
	}

	Matrix4 Matrix4::operator*(Matrix4& other)
	{
		Matrix4 r;
		r[0][0] = dot(this->row(0), other.col(0));
		r[0][1] = dot(this->row(0), other.col(1));
		r[0][2] = dot(this->row(0), other.col(2));
		r[0][3] = dot(this->row(0), other.col(3));
		r[1][0] = dot(this->row(1), other.col(0));
		r[1][1] = dot(this->row(1), other.col(1));
		r[1][2] = dot(this->row(1), other.col(2));
		r[1][3] = dot(this->row(1), other.col(3));
		r[2][0] = dot(this->row(2), other.col(0));
		r[2][1] = dot(this->row(2), other.col(1));
		r[2][2] = dot(this->row(2), other.col(2));
		r[2][3] = dot(this->row(2), other.col(3));
		r[3][0] = dot(this->row(3), other.col(0));
		r[3][1] = dot(this->row(3), other.col(1));
		r[3][2] = dot(this->row(3), other.col(2));
		r[3][3] = dot(this->row(3), other.col(3));

		return r;
	}
}
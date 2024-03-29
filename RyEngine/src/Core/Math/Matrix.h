#pragma once
#include "Vector.h"

namespace RE
{
	class Matrix3
	{
		friend Vector3 operator*(Vector3& v, Matrix3& m);

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

		Vector3 row(UINT i);
		Vector3 col(UINT i);
		Vector3& operator[](UINT i);
		Matrix3 operator*(Matrix3& r);
		Vector3 operator*(Vector3& v);
		inline int GetRowCount() { return 3; }
		inline int GetColCount() { return 3; }



	private:
		Vector3 _arr[3];
	};

	class Matrix4
	{
		friend Vector4 operator*(Vector4& v, Matrix4& m);

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
		Vector4 operator*(Vector4& v);
		inline int GetRowCount() const { return 4; }
		inline int GetColCount() const { return 4; }

		template<typename T>
		T GetMatrix(int startRow, int startCol)
		{
			//TODO currently can only return 3x3 matrix since I haven't implemented a smaller one
			T m;
			int rowItt = 0, colItt = 0;

			for (int i = startRow; i < startRow + m.GetRowCount(); i++)
			{
				for (int j = startCol; j < startCol + m.GetColCount(); j++)
				{
					m[rowItt][colItt] = _arr[i][j];
					colItt++;
				}
				colItt = 0;
				rowItt++;
			}

			return m;
		}

	private:
		Vector4 _arr[4];
	};

}
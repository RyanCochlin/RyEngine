#include "pch.h"
#include "Transform.h"
#include "Core/Math/CoreMath.h"

namespace RE
{
	Transform::Transform() :
		_mTranslation{ 0, 0, 0 },
		_mRotation{ 0, 0, 0 },
		_mScale{ 1, 1, 1 }
	{}

	Vector3 Transform::GetTranslation()
	{
		return _mTranslation;
	}

	Vector3 Transform::GetRotation()
	{
		//TODO this is going to be hard 
		return _mRotation;
	}

	Vector3 Transform::GetScale()
	{
		Matrix3 scale = _mTRSMatrix.GetMatrix<Matrix3>(0, 0);
		float scaleX = magnitutde(scale.row(0));
		float scaleY = magnitutde(scale.row(1));
		float scaleZ = magnitutde(scale.row(2));
		return { scaleX, scaleY, scaleZ };
	}

	void Transform::Translate(Vector3 translation)
	{
		_mTranslation = translation;
	}

	void Transform::Rotate(Vector3 rotation)
	{
		_mRotation = rotation;
	}

	void Transform::Scale(Vector3 scale)
	{
		_mScale = scale;
	}

	Matrix4 Transform::GetWorld()
	{
		//TODO this is pretty inefficient. Need to look at this again
		Matrix4 T;
		T[0][3] = _mTranslation.x;
		T[1][3] = _mTranslation.y;
		T[2][3] = _mTranslation.z;

		Matrix4 S;
		S[0][0] = _mScale.x;
		S[1][1] = _mScale.y;
		S[2][2] = _mScale.z;

		float xRad = Math::degToRad(_mRotation.x);
		float yRad = Math::degToRad(_mRotation.y);
		float zRad = Math::degToRad(_mRotation.z);
		
		Matrix4 rx;
		rx[1][1] = cos(xRad);
		rx[1][2] = -sin(xRad);
		rx[2][1] = sin(xRad);
		rx[2][2] = cos(xRad);

		Matrix4 ry;
		ry[0][0] = cos(yRad);
		ry[0][2] = sin(xRad);
		ry[2][0] = -sin(xRad);
		ry[2][2] = cos(xRad);

		Matrix4 rz;
		rz[0][0] = cos(yRad);
		rz[0][1] = -sin(xRad);
		rz[1][0] = sin(xRad);
		rz[1][1] = cos(xRad);

		Matrix4 R = rx * ry * rz;

		return T * S * R;
	}
}

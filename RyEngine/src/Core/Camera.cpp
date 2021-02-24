#include "pch.h"
#include "Camera.h"
#include "SubSystemManager.h"

namespace RE
{
	Camera::Camera() :
		_mView(),
		_mProj(),
		_mPos(),
		_mUp(),
		_mLookAt(),
		_mNear(0.0f),
		_mFar(0.0f),
		_mNearHeight(0.0f),
		_mFarHeight(0.0f),
		_mAspect(0.0f),
		_mDirty(false)
	{
		SubSystemManager::Instance().Cams()->AddCamera(this);
	}

	Vector3 Camera::GetPosition() const
	{
		return _mPos;
	}
	
	void Camera::SetPosition(Vector3 pos)
	{
		_mPos = pos;
		_mDirty = true;
	}

	Vector3 Camera::GetUp() const
	{
		return _mUp;
	}

	float Camera::GetNear() const
	{
		return _mNear;
	}

	float Camera::GetFar() const
	{
		return _mFar;
	}

	Matrix4 Camera::GetModelView() const
	{
		return _mViewProj;
	}

	void Camera::LookAt(Vector3 pos, Vector3 target, Vector3 worldUp)
	{
		_mDirty = true;
		_mPos = pos;

		_mLookAt = normalize(target - pos);
		Vector3 right = normalize( cross(worldUp, _mLookAt) );
		_mUp = cross(_mLookAt, right);
	}

	//----------------------------------------------------------------------------------------//
	OrthographicCamera::OrthographicCamera() :
		Camera(),
		_mLeft(0),
		_mRight(0),
		_mTop(0),
		_mBottom(0)
	{}

	OrthographicCamera::~OrthographicCamera()
	{
		Camera::~Camera();
	}

	void OrthographicCamera::SetLens(float leftPlane, float rightPlane, float topPlane, float bottomPlane, float nearPlane, float farPlane)
	{
		_mDirty = true;
		_mLeft = leftPlane;
		_mRight = rightPlane;
		_mTop = topPlane;
		_mBottom = bottomPlane;
		_mNear = nearPlane;
		_mFar = farPlane;
	}

	void OrthographicCamera::Update()
	{
		if (!_mDirty)
			return;

		CalculateProj();
		CalculateView();

		_mViewProj = _mView * _mProj;
		_mDirty = false;
	}

	void OrthographicCamera::CalculateView()
	{
		Vector3 z = _mLookAt;
		Vector3 y = _mUp;
		Vector3 x = cross(z, y);
		_mView = Matrix4{
			Vector4{x.x, x.y, x.z, 0.0f/*-_mPos.x*/},
			Vector4{y.x, y.y, y.z, 0.0f/*-_mPos.y*/},
			Vector4{z.x, z.y, z.z, 0.0f/*-_mPos.z*/},
			Vector4{-_mPos.x, -_mPos.y, -_mPos.z, 1.0}
			//Vector4{0.0f, 0.0f, 0.0f, 1.0}
		};
	}

	void OrthographicCamera::CalculateProj()
	{
		float wx = (_mRight + _mLeft) / (_mRight - _mLeft);
		float wy = (_mTop + _mBottom) / (_mTop - _mBottom);
		float wz = (_mFar + _mNear) / (_mFar - _mNear);
		wx *= (wx == 0) ? 1.0 : -1.0;
		wy *= (wy == 0) ? 1.0 : -1.0;
		wz *= (wz == 0) ? 1.0 : -1.0;

		Vector4 x{ 2.0f / (_mRight - _mLeft), 0.0f, 0.0f, 0.0f };
		Vector4 y{ 0.0f, 2.0f / (_mTop - _mBottom), 0.0f, 0.0f };
		Vector4 z{ 0.0f, 0.0f, 2.0f / (_mFar - _mNear), 0.0f };
		Vector4 w{ wx, wy, wz, 1.0f };
		_mProj = Matrix4(x, y, z, w);
	}

	//perspective set lens
	/*_mDirty = true;
	_mAspect = aspect;
	_mNear = zNear;
	_mFar = zFar;

	float width = _mFarHeight * _mAspect;
	float halfWidth = width * 0.5f;
	_mLeft = _mPos.x - halfWidth;
	_mRight = _mPos.x + halfWidth;
	_mTop =*/
}
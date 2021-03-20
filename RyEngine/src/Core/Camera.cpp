#include "pch.h"
#include <math.h>
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
		_mWidth(0),
		_mHeight(0)
	{}

	OrthographicCamera::~OrthographicCamera()
	{
		Camera::~Camera();
	}

	void OrthographicCamera::SetLens(float width, float height, float nearPlane, float farPlane)
	{
		_mDirty = true;
		_mWidth = width;
		_mHeight = height;
		_mNear = nearPlane;
		_mFar = farPlane;
	}

	void OrthographicCamera::Update()
	{
		if (!_mDirty)
			return;

		CalculateProj();
		CalculateView();

		_mViewProj = _mProj * _mView;
		_mDirty = false;
	}

	void OrthographicCamera::CalculateView()
	{
		Vector3 z = _mLookAt;
		Vector3 y = _mUp;
		Vector3 x = cross(z, y);
		_mView = Matrix4{
			Vector4{x.x, x.y, x.z, -_mPos.x},
			Vector4{y.x, y.y, y.z, -_mPos.y},
			Vector4{z.x, z.y, z.z, -_mPos.z},
			Vector4{0.0f, 0.0f, 0.0f, 1.0}
		};
	}

	void OrthographicCamera::CalculateProj()
	{
		float right = _mWidth / 2.0f;
		float left = -right;
		float top = _mHeight / 2.0f;
		float bottom = -top;
		float wx = (right + left) / (right - left);
		float wy = (top + bottom) / (top - bottom);
		float wz = (_mFar + _mNear) / (_mFar - _mNear);
		wx *= (wx == 0) ? 1.0 : -1.0;
		wy *= (wy == 0) ? 1.0 : -1.0;
		wz *= (wz == 0) ? 1.0 : -1.0;

		Vector4 x{ 2.0f / (right - left), 0.0f, 0.0f, wx };
		Vector4 y{ 0.0f, 2.0f / (top - bottom), 0.0f, wy };
		Vector4 z{ 0.0f, 0.0f, 2.0f / (_mFar - _mNear), wz };
		Vector4 w{ 0.0f, 0.0f, 0.0f, 1.0f };
		_mProj = Matrix4(x, y, z, w);
	}

	//----------------------------------------------------------------------------------------//
	PerspectiveCamera::PerspectiveCamera() :
		_mNearHeight(0.0f),
		_mFarHeight(0.0f)
	{}

	PerspectiveCamera::~PerspectiveCamera()
	{
		Camera::~Camera();
	}

	void PerspectiveCamera::Update()
	{
		if (!_mDirty)
			return;

		CalculateProj();
		CalculateView();

		_mViewProj = _mProj * _mView;
		_mDirty = false;
	}

	void PerspectiveCamera::CalculateView()
	{
		Vector3 z = _mLookAt;
		Vector3 y = _mUp;
		Vector3 x = cross(z, y);
		_mView = Matrix4{
			Vector4{x.x, x.y, x.z, -_mPos.x},
			Vector4{y.x, y.y, y.z, -_mPos.y},
			Vector4{z.x, z.y, z.z, -_mPos.z},
			Vector4{0.0f, 0.0f, 0.0f, 1.0}
		};
	}
	
	void PerspectiveCamera::CalculateProj()
	{
		float y = 1.0f * tanf(_mFovY * 0.5f);
		float x = y * _mAspect;
		float q1 = _mFar / (_mNear - _mFar);
		float q2 = q1 * _mNear;

		_mProj = Matrix4(
			Vector4{ x, 0.0f, 0.0f, 0.0f }, 
			Vector4{ 0.0f, y, 0.0f, 0.0f }, 
			Vector4{ 0.0f, 0.0f, q1, -1.0f }, 
			Vector4{ 0.0f, 0.0f, q2, 0.0f }
		);
	}

	void PerspectiveCamera::SetLens(float fovY, float aspect, float zNear, float zFar)
	{
		//perspective set lens
		_mDirty = true;
		_mAspect = aspect;
		_mFovY = fovY;
		_mNear = zNear;
		_mFar = zFar;
	}

}
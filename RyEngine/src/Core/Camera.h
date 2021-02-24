#pragma once

#include "pch.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace RE
{
	class RE_API Camera
	{
	public:
		Camera();
		virtual ~Camera() {}

		virtual Vector3 GetPosition() const;
		virtual void SetPosition(Vector3 pos);
		virtual Vector3 GetUp() const;
		virtual float GetNear() const;
		virtual float GetFar() const;
		virtual Matrix4 GetModelView() const;

		virtual void LookAt(Vector3 pos, Vector3 target, Vector3 worldUp);
		//virtual void SetLens(float fovY, float aspect, float zNear, float zFar) = 0;

		virtual void Update() = 0;

	protected:
		virtual void CalculateView() = 0;
		virtual void CalculateProj() = 0;

		Matrix4 _mView;
		Matrix4 _mProj;
		Matrix4 _mViewProj;
		Vector3 _mPos;
		Vector3 _mUp;
		Vector3 _mLookAt;
		float _mNear;
		float _mFar;
		float _mNearHeight;
		float _mFarHeight;
		float _mAspect;
		bool _mDirty;
	};

	//-------------------------------------------------//
	class RE_API OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera();
		virtual ~OrthographicCamera();

		void SetLens(float leftPlane, float rightPlane, float topPlane, float bottomPlane, float nearPlane, float farPlane);

		virtual void Update() override;

	protected:
		virtual void CalculateView() override;
		virtual void CalculateProj() override;

	private:
		float _mLeft;
		float _mRight;
		float _mTop;
		float _mBottom;
	};

	//------------------------------------------------//
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera();
		virtual ~PerspectiveCamera();

		//void SetLens(float fovY, float aspect, float zNear, float zFar) = 0;

		virtual void Update() override;
	};
}

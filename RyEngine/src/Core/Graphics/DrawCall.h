#pragma once
#include <vector>
#include "Core/Math/Matrix.h"
#include "Core/Graphics/Light.h"

namespace RE
{
	class DrawCall
	{
	public:
		DrawCall();

		void SetMVP(Matrix4& mvp);
		Matrix4 GetMVP();

		void SetAmbient(Vector4& ambient);
		Vector4 GetAmbient();

		void SetEyePosition(Vector3& position);
		Vector3 GetEyePosition();

		void AddLight(Light* light);
		virtual std::vector<Light*>& GetLights();


	protected:
		Matrix4 _mModelViewProjection;
		Vector4 _mAmbientLight;
		Vector3 _mEyePosition;
		std::vector<Light*> _mLights;
	};
}

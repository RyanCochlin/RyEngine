#pragma once

#include "Core/Math/Vector.h"

namespace RE
{

class LightManager
{
public:
	Vector4 GetAmbientLight() const;
	void SetAmbientLight(Vector4 ambientLight);

private:
	Vector4 _mAmbientLight;
};

}
#pragma once

#include <vector>
#include "Core/GlobalDefines.h"
#include "Core/Math/Vector.h"
#include "Light.h"

namespace RE
{

class LightManager
{
public:
	LightManager();
	~LightManager();

	Vector4 GetAmbientLight() const;
	void SetAmbientLight(Vector4 ambientLight);

	void AddLight(Light* light);
	std::vector<Light*>& GetLightInstances();
	const std::vector<Light*>& GetLights();

private:
	Vector4 _mAmbientLight;
	std::vector<Light*> _mLights;
};

}
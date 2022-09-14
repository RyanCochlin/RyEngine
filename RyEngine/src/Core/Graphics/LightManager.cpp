#include "pch.h"
#include "LightManager.h"

namespace RE
{

Vector4 LightManager::GetAmbientLight() const
{
	return _mAmbientLight;
}

void LightManager::SetAmbientLight(Vector4 ambientLight)
{
	_mAmbientLight = ambientLight;
}

}
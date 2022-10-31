#include "pch.h"
#include "LightManager.h"
#include "Core/SubSystemManager.h"

namespace RE
{

LightManager::LightManager() :
	_mAmbientLight{ 0.5f, 0.5f, 0.5f, 1.0f }
{}

LightManager::~LightManager()
{
	for (auto light : _mLights)
	{
		delete light;
	}
}

const std::vector<Light*>& LightManager::GetLights()
{
	return _mLights;
}

Vector4 LightManager::GetAmbientLight() const
{
	return _mAmbientLight;
}

void LightManager::SetAmbientLight(Vector4 ambientLight)
{
	_mAmbientLight = ambientLight;
}

void LightManager::AddLight(Light* light)
{
	_mLights.push_back(light);
}

std::vector<Light*>& LightManager::GetLightInstances()
{
	return _mLights;
}

}
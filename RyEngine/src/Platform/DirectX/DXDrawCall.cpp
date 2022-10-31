#include "pch.h"
#include <algorithm>
#include "DXDrawCall.h"
#include "Core/Graphics/Light.h"
#include "Resources/ResColored.h"

namespace RE
{
	DXDrawCall::DXDrawCall() :
		_mDirectionalLightCount(0)
	{}

	DXDrawCall::DXDrawCall(DrawCall dc)
	{
		_mModelViewProjection = dc.GetMVP();
		_mAmbientLight = dc.GetAmbient();
		_mEyePosition = dc.GetEyePosition();
		_mDirectionalLightCount = 0;

		std::vector<Light*> lights = dc.GetLights();
		std::sort(lights.begin(), lights.end(), [](const Light* a, const Light* b) {
			return a->Type > b->Type;
		});

		uint32_t lightCount = 0;
		for (auto& light : lights)
		{
			DxLight dxLight(light);
			_mLights.push_back(dxLight);
			_mDirectionalLightCount += (light->Type == LightType::Directional) ? 1 : 0;
			//TODO other light types

			//TODO get pass type and max lights from draw call
			if (++lightCount >= MAX_LIGHTS)
				break;
		}
	}

	uint32_t DXDrawCall::GetDirectionalLightCount()
	{
		return _mDirectionalLightCount;
	}

	std::vector<DxLight>& DXDrawCall::GetDxLights()
	{
		return _mLights;
	}
}
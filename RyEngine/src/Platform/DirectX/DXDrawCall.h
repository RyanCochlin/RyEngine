#pragma once

#include "Core/Graphics/DrawCall.h"
#include "Platform/DirectX/Resources/DxLight.h"

namespace RE
{
	class DXDrawCall : public DrawCall
	{
	public:
		DXDrawCall();
		DXDrawCall(DrawCall dc);

		// TODO make light base class for this
		std::vector<DxLight>& GetDxLights();
		uint32_t GetDirectionalLightCount();
		uint32_t GetPointLightCount();

		uint32_t GetDirectionalLightIndex();
		uint32_t GetPointLightIndex();

	private:
		uint32_t _mDirectionalLightCount;
		uint32_t _mPointLightCount;
		std::vector<DxLight> _mLights;
	};
}
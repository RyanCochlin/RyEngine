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

	private:
		uint32_t _mDirectionalLightCount;
		std::vector<DxLight> _mLights;
	};
}
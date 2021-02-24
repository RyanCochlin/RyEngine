#pragma once

#include "Core/Graphics/DrawCall.h"

namespace RE
{
	class DXDrawCall : public DrawCall
	{
	public:
		DXDrawCall();
		DXDrawCall(DrawCall dc);
	};
}
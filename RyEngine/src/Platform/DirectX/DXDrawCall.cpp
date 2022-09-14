#include "pch.h"
#include "DXDrawCall.h"

namespace RE
{
	DXDrawCall::DXDrawCall()
	{}

	DXDrawCall::DXDrawCall(DrawCall dc)
	{
		_mModelViewProjection = dc.GetMVP();
		_mAmbientLight = dc.GetAmbient();
	}
}
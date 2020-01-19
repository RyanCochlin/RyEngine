#pragma once

#include "SubSystem.h"

namespace RyEngine
{
	class RenderableSubSystem : public SubSystem
	{
	public:
		virtual void OnRender() = 0;
	};
}
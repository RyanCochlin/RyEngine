#pragma once

#include "SubSystem.h"

namespace RyEngine
{
	class UpdatableSubSystem : public SubSystem
	{
	public:
		virtual void OnUpdate() = 0;
	};
}
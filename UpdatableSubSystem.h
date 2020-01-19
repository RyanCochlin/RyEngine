#pragma once

#include "stdafx.h"

#include "SubSystem.h"

namespace RyEngine
{
	class UpdatableSubSystem : public SubSystem
	{
	public:
		virtual void OnUpdate() = 0;
	};
}
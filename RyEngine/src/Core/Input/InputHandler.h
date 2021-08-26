#pragma once

#include <bitset>
#include "Core/Events/Event.h"
#include "Core/Events/KeyboardEvents.h"
#include "Core/Events/MouseEvents.h"

namespace RE
{
	class InputHandler
	{
	public:
		InputHandler() = default;
		virtual ~InputHandler() {}
	};
}
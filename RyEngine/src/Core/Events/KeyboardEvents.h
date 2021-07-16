#pragma once

#include "Event.h"

namespace RE
{
	class KeyDownEvent : public Event
	{
	public:
		DECLARE_EVENT_CLASS(RE_KEY_DOWN)

		virtual std::string ToString() const override
		{
			// TODO print key code
		}
	};
}
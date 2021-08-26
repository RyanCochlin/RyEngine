#pragma once

#include "Event.h"
#include "Core/Input/KeyCodes.h"

namespace RE
{
	class KeyboardEvent : public Event
	{
	public:
		void SetKeyCode(RE_KEYCODE keycode)
		{
			_mKeycode = keycode;
		}

		RE_KEYCODE GetKeyCode() const
		{
			return _mKeycode;
		}

	protected:
		RE_KEYCODE _mKeycode;
	};

	class KeyDownEvent : public KeyboardEvent
	{
	public:
		DECLARE_EVENT_CLASS(RE_KEY_DOWN)

		virtual std::string ToString() const override
		{
			char ch = static_cast<char>(_mKeycode);
			return std::string(&ch);
		}
	};

	class KeyUpEvent : public KeyboardEvent
	{
	public:
		DECLARE_EVENT_CLASS(RE_KEY_UP)

		virtual std::string ToString() const override
		{
			char ch = static_cast<char>(_mKeycode);
			return std::string(&ch);
		}
	};
}
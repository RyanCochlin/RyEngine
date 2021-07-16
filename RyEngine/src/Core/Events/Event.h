#pragma once

#include <string>

namespace RE
{
	enum class EventType
	{
		RE_NONE = 0,

		//window types
		RE_WINDOW_CLOSE,
		RE_WINDOW_RESIZE,

		//keyboard types
		RE_KEY_DOWN,
		RE_KEY_UP,

		//mouse types
		RE_MOUSE_DOWN,
		RE_MOUSE_UP,
		RE_MOUSE_MOVE,
		RE_MOUSE_SCROLL
	};

#define DECLARE_EVENT_CLASS(type) static EventType StaticEventType() { return EventType::##type; }\
									virtual EventType GetEventType() const override { return StaticEventType(); }\
									virtual const char* GetEventTypeName() const override { return #type; }

	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetEventTypeName() const = 0;
		virtual std::string ToString() const { return GetEventTypeName(); }
	};
}
#pragma once

#include "Event.h"
#include "Core/Math/Vector.h"

namespace RE
{
	class MouseEvent : public Event
	{
	public:
		MouseEvent() :
			_mPosition({0.0f, 0.0f})
		{}

		float GetX() const { return _mPosition.x; }
		float GetY() const { return _mPosition.y; }
		Vector2 GetPosition() const { return _mPosition; }
		void SetPosition(Vector2 pos) { _mPosition = pos; }

	protected:
		Vector2 _mPosition;
	};

	class MouseDownEvent : public MouseEvent
	{
	public:
		DECLARE_EVENT_CLASS(RE_MOUSE_DOWN)
	};

	class MouseUpEvent : public MouseEvent
	{
	public:
		DECLARE_EVENT_CLASS(RE_MOUSE_UP)
	};

	class MouseMoveEvent : public MouseEvent
	{
	public:
		DECLARE_EVENT_CLASS(RE_MOUSE_MOVE)
	};

	class MouseScrollEvent : public MouseEvent
	{
	public:
		DECLARE_EVENT_CLASS(RE_MOUSE_SCROLL);

		RE_API short GetWheelDelta() const { return _mWheelDelta; }
		void SetWheelDelta(int delta) { _mWheelDelta = delta; }

	private:
		short _mWheelDelta;
	};
}
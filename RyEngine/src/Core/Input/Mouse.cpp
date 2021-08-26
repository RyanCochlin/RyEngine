#include "pch.h"
#include "Mouse.h"
#include "Core/SubSystemManager.h"

namespace RE
{
	void Mouse::AddMouseEventHandler(std::function<void(MouseEvent&, EventType)> callback, EventType eventType)
	{
		Mouse* mouse = SubSystemManager::Instance().GetSubSystem<InputSystem>()->GetMouse();
		mouse->SetMouseEventHandler(callback, eventType);
	}

	void Mouse::SetMouseEventHandler(OnMouseEventCallback callback, EventType eventType)
	{
		ASSERT(eventType == EventType::RE_MOUSE_DOWN || eventType == EventType::RE_MOUSE_UP || eventType == EventType::RE_MOUSE_MOVE || eventType == EventType::RE_MOUSE_SCROLL);

		_mEventCallbacks[eventType].push_back(callback);
	}

	Mouse::Mouse() :
		_mEventCallbacks()
	{}

	void Mouse::OnMouseDown(MouseEvent& mouseEvent)
	{
		RE_CORE_WARN("Mouse down event at {0}:{1}", mouseEvent.GetX(), mouseEvent.GetY());
		FireMouseEvent(EventType::RE_MOUSE_DOWN, mouseEvent);
	}

	void Mouse::OnMouseUp(MouseEvent& mouseEvent)
	{
		RE_CORE_WARN("Mouse up event at {0}:{1}", mouseEvent.GetX(), mouseEvent.GetY());
		FireMouseEvent(EventType::RE_MOUSE_UP, mouseEvent);
	}
	
	void Mouse::OnMouseMove(MouseEvent& mouseEvent)
	{
		//this printing was crazy
		//RE_CORE_WARN("Mouse move event at {0}:{1}", mouseEvent.GetX(), mouseEvent.GetY());
		FireMouseEvent(EventType::RE_MOUSE_MOVE, mouseEvent);
	}

	void Mouse::OnMouseScroll(MouseEvent& mouseEvent)
	{
		MouseScrollEvent& scroll = dynamic_cast<MouseScrollEvent&>(mouseEvent);
		RE_CORE_WARN("Mouse scroll event at {0}:{1} with delta {2}", mouseEvent.GetX(), mouseEvent.GetY(), scroll.GetWheelDelta());
		FireMouseEvent(EventType::RE_MOUSE_SCROLL, mouseEvent);
	}

	void Mouse::FireMouseEvent(EventType eventType, MouseEvent& mouseEvent)
	{
		if (_mEventCallbacks.find(eventType) != _mEventCallbacks.end())
		{
			for (auto callback : _mEventCallbacks[eventType])
			{
				callback(mouseEvent, eventType);
			}
		}
	}
}
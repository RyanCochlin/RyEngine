#pragma once

#include <vector>
#include <map>
#include <functional>
#include "InputHandler.h"

namespace RE
{
	class Mouse
	{
	public:
		using OnMouseEventCallback = std::function<void(MouseEvent&, EventType)>;

		RE_API static void AddMouseEventHandler(std::function<void(MouseEvent&, EventType)> callback, EventType eventType);

		Mouse();

		void OnMouseDown(MouseEvent& mouseEvent);
		void OnMouseUp(MouseEvent& mouseEvent);
		void OnMouseMove(MouseEvent& mouseEvent);
		void OnMouseScroll(MouseEvent& mouseEvent);
		void SetMouseEventHandler(OnMouseEventCallback callback, EventType eventType);

	private:
		void FireMouseEvent(EventType eventType, MouseEvent& mouseEvent);

		std::unordered_map<EventType, std::vector<OnMouseEventCallback>> _mEventCallbacks;
	};
}

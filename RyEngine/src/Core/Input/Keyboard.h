#pragma once

#include "InputHandler.h"

namespace RE
{
	class Keyboard : public InputHandler
	{
	public:
		typedef void (*OnKeyboardEventCallback)(KeyboardEvent*);

		RE_API static bool KeyDown(RE_KEYCODE keyCode);

		static bool AutoRepeatEnabled()
		{
			//TODO this value should be in settings eventually
			return _sAutoRepeat;
		}

		bool IsKeyDown(RE_KEYCODE keyCode);
		void OnKeyDown(KeyDownEvent& e);
		void OnKeyUp(KeyUpEvent& e);

		RE_API static void AddKeyboardEventHander(OnKeyboardEventCallback callback);

	private:
		static const bool _sAutoRepeat = false;
		static const unsigned int _sKeys = 256u;
		std::bitset<_sKeys> _sKeyStates;
	};
}

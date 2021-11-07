#include "pch.h"
#include "Keyboard.h"
#include "Core/SubSystemManager.h"

namespace RE
{
	bool Keyboard::KeyDown(RE_KEYCODE keyCode)
	{
		return SubSystemManager::Instance().GetSubSystem<InputSystem>()->GetKeyboard()->IsKeyDown(keyCode);
	}

	bool Keyboard::IsKeyDown(RE_KEYCODE keyCode)
	{
		return _sKeyStates[keyCode];
	}

	void Keyboard::OnKeyDown(KeyDownEvent& e)
	{
		RE_KEYCODE keycode = e.GetKeyCode();
		// TODO delete this logging
		if (e.GetKeyCode() >= 'A' && e.GetKeyCode() <= 'Z')
		{
			char c[2] = { keycode, 0 };
			RE_LOG("Key down: {0}", c);
		}
		else
		{
			RE_LOG("Key down: {0}", keycode);
		}

		_sKeyStates[keycode] = true;
	}

	void Keyboard::OnKeyUp(KeyUpEvent& e)
	{
		RE_KEYCODE keycode = e.GetKeyCode();
		// TODO delete logging code
		if (e.GetKeyCode() >= 'A' && e.GetKeyCode() <= 'Z')
		{
			char c[2] = { keycode, 0 };
			RE_LOG("Key up: {0}", c);
		}
		else
		{
			RE_LOG("Key up: {0}", keycode);
		}
		_sKeyStates[keycode] = false;
	}
}
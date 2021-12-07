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
		_sKeyStates[keycode] = true;
	}

	void Keyboard::OnKeyUp(KeyUpEvent& e)
	{
		RE_KEYCODE keycode = e.GetKeyCode();
		_sKeyStates[keycode] = false;
	}
}
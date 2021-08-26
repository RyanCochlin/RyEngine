#include "pch.h"
#include "InputSystem.h"
#include "Core/SubSystemManager.h"
#include "Core/Window/Window.h"
#include "Keyboard.h"
#include "Mouse.h"

namespace RE
{
	void InputSystem::OnStart()
	{
		Window* mainWindow = SubSystemManager::Instance().GetSubSystem<WindowSystem>()->GetMainWindow();
		mainWindow->SetEventCallback(OnWindowEvent, this);

		_mKeyboard = new Keyboard();
		_mMouse = new Mouse();
	}

	void InputSystem::Release()
	{
		delete _mKeyboard;
		delete _mMouse;
	}

	Keyboard* InputSystem::GetKeyboard()
	{
		return SubSystemManager::Instance().GetSubSystem<InputSystem>()->_mKeyboard;
	}

	Mouse* InputSystem::GetMouse()
	{
		return SubSystemManager::Instance().GetSubSystem<InputSystem>()->_mMouse;
	}

	void InputSystem::OnWindowEvent(Event& e, void* obj)
	{
		InputSystem* input = static_cast<InputSystem*>(obj);
		input->DispatchWindowEvent(e);
	}

	void InputSystem::DispatchWindowEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.DispatchEvent<KeyDownEvent>(std::bind(&Keyboard::OnKeyDown, _mKeyboard, std::placeholders::_1));
		dispatcher.DispatchEvent<KeyUpEvent>(std::bind(&Keyboard::OnKeyUp, _mKeyboard, std::placeholders::_1));

		dispatcher.DispatchEvent<MouseDownEvent>(std::bind(&Mouse::OnMouseDown, _mMouse, std::placeholders::_1));
		dispatcher.DispatchEvent<MouseUpEvent>(std::bind(&Mouse::OnMouseDown, _mMouse, std::placeholders::_1));
		dispatcher.DispatchEvent<MouseMoveEvent>(std::bind(&Mouse::OnMouseMove, _mMouse, std::placeholders::_1));
		dispatcher.DispatchEvent<MouseScrollEvent>(std::bind(&Mouse::OnMouseScroll, _mMouse, std::placeholders::_1));
	}
}
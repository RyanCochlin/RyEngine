#pragma once

#include "Core/SubSystem.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Core/Events/Event.h"

namespace RE
{
	class InputSystem : public SubSystem
	{
	public:
		RE_DELCARE_SUBSYSTEM(InputSystem)

		virtual void OnRegister() override {}
		virtual void OnStart() override;
		virtual void OnUpdate() override {}
		virtual void OnRender() override {}
		virtual void Release() override;
		Keyboard* GetKeyboard();
		Mouse* GetMouse();

		static void OnWindowEvent(Event& e, void* obj);

	private:
		void DispatchWindowEvent(Event& e);

		Keyboard* _mKeyboard;
		Mouse* _mMouse;
	};
}


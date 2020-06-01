#pragma once

#define RE_SUBSYSTEM_OVERRIDE void OnRegister() override; \
void OnStart() override; \
void OnUpdate() override; \
void OnRender() override; \
void Release() override;

namespace RE
{
	class SubSystem
	{
	public:
		virtual void OnRegister() = 0;
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void Release() = 0;
	protected:
	};
}


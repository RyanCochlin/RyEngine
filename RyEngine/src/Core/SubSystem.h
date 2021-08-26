#pragma once

#define RE_DELCARE_SUBSYSTEM(type) static SubSystemType GetStaticType() { return SubSystemType::##type; }\
									virtual SubSystemType GetSubSystemType() const override { return GetStaticType(); }

namespace RE
{
	enum class SubSystemType
	{
		RE_NONE = 0,
		WindowSystem,
		InputSystem,
		LoggingSystem,
		GraphicsSystem,
		CameraSystem
	};

	class SubSystem
	{
	public:
		virtual SubSystemType GetSubSystemType() const = 0;
		virtual void OnRegister() = 0;
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void Release() = 0;
	};
}


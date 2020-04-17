#include "pch.h"
#include "Log.h"

namespace RE
{
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;

	void Log::OnRegister()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		sCoreLogger = spdlog::stdout_color_mt("RyEngine");
		sCoreLogger->set_level(spdlog::level::trace);

		sClientLogger = spdlog::stdout_color_mt("Application");
		sClientLogger->set_level(spdlog::level::trace);
	}

	void Log::OnStart() {}
	void Log::Release() {}
	void Log::OnRender() {}
	void Log::OnUpdate() {}
}
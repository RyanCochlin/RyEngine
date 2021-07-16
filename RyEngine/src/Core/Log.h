#pragma once

#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "GlobalDefines.h"
#include "SubSystem.h"

namespace RE
{
	class RE_API Log : public SubSystem
	{
	public:
		RE_DELCARE_SUBSYSTEM(LoggingSystem)

		void OnRegister() override;
		void OnStart() override;
		void OnUpdate() override;
		void OnRender() override;
		void Release() override;

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> sCoreLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
	};
}

#define RE_CORE_ERROR(...) ::RE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RE_CORE_WARN(...) ::RE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RE_CORE_INFO(...) ::RE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RE_CORE_LOG(...) ::RE::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define RE_LOG_ERROR(...) ::RE::Log::GetClientLogger()->error(__VA_ARGS__)
#define RE_LOG_WARN(...) ::RE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RE_LOG_INFO(...) ::RE::Log::GetClientLogger()->info(__VA_ARGS__)
#define RE_LOG(...) ::RE::Log::GetClientLogger()->trace(__VA_ARGS__)
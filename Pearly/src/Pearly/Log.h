#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Pearly {
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}


// core log
#define PR_CORE_TRACE(...) ::Pearly::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PR_CORE_INFO(...) ::Pearly::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PR_CORE_WARN(...) ::Pearly::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PR_CORE_ERROR(...) ::Pearly::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PR_CORE_FATAL(...) ::Pearly::Log::GetCoreLogger()->fatal(__VA_ARGS__)


// client log
#define PR_TRACE(...) ::Pearly::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PR_INFO(...) ::Pearly::Log::GetClientLogger()->info(__VA_ARGS__)
#define PR_WARN(...) ::Pearly::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PR_ERROR(...) ::Pearly::Log::GetClientLogger()->error(__VA_ARGS__)
#define PR_FATAL(...) ::Pearly::Log::GetClientLogger()->fatal(__VA_ARGS__)
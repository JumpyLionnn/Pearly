#pragma once
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

#include "glm/gtx/string_cast.hpp"

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

// logging types that are not defined by spglog
template<glm::length_t L, typename T, glm::qualifier Q>
inline std::ostream& operator<<(std::ostream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline std::ostream& operator<<(std::ostream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

// core log
#if PR_ENABLE_CORE_LOG
	#define PR_CORE_TRACE(...) ::Pearly::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define PR_CORE_INFO(...) ::Pearly::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define PR_CORE_WARN(...) ::Pearly::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define PR_CORE_ERROR(...) ::Pearly::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define PR_CORE_CRITICAL(...) ::Pearly::Log::GetCoreLogger()->critical(__VA_ARGS__)
#else
	#define PR_CORE_TRACE(...) 
	#define PR_CORE_INFO(...) 
	#define PR_CORE_WARN(...) 
	#define PR_CORE_ERROR(...) 
	#define PR_CORE_CRITICAL(...) 
#endif


// client log
#define PR_TRACE(...) ::Pearly::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PR_INFO(...) ::Pearly::Log::GetClientLogger()->info(__VA_ARGS__)
#define PR_WARN(...) ::Pearly::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PR_ERROR(...) ::Pearly::Log::GetClientLogger()->error(__VA_ARGS__)
#define PR_CRITICAL(...) ::Pearly::Log::GetClientLogger()->critical(__VA_ARGS__)
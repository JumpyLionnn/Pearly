#pragma once
#include <memory>

#ifdef PR_PLATFORM_WINDOWS

#else
	#error Pearly only supports Windows!
#endif

#ifdef PR_ENABLE_ASSERTS
	#define PR_CORE_ASSERT(x, ...) {if(!(x)) { PR_CORE_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak();}}
	#define PR_ASSERT(x, ...) {if(!(x)) { PR_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak();}}
#else
	#define PR_CORE_ASSERT(x, ...)
	#define PR_ASSERT(x, ...)
#endif

#ifdef PR_ENABLE_RENDERER_LOG_LEVEL 
	#define PR_ENABLE_RENDERER_LOG 
#endif 
#ifdef PR_ENABLE_RENDERER_LOG 
	#ifndef PR_ENABLE_CORE_LOG 
		// We do require the core logger
		#define PR_ENABLE_CORE_LOG 
	#endif 
	#ifndef PR_ENABLE_RENDERER_LOG_LEVEL 
		// Default: warning and above 
		#define PR_ENABLE_RENDERER_LOG_LEVEL 2
	#endif 
#endif

#define PR_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (1 << x)



namespace Pearly {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;


	using uint32 = uint32_t;
	using uint8 = uint8_t;
}
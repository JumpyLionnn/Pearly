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
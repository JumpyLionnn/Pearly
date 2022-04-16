#pragma once


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

#define BIT(x) (1 << x)
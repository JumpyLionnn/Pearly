#pragma once
#include "CoreConfig.h"
#include <memory>

#if !defined(PR_PLATFORM_WINDOWS) && !defined(PR_PLATFORM_IOS) && !defined(PR_PLATFORM_MACOS) && !defined(PR_PLATFORM_ANDROID) && !defined(PR_PLATFORM_LINUX)
	#ifdef _WIN32
		/* Windows x64/x86 */
		#ifdef _WIN64
			/* Windows x64  */
			#define PR_PLATFORM_WINDOWS
		#else
			/* Windows x86 */
			#error "x86 Builds are not supported!"
		#endif
	#elif defined(__APPLE__) || defined(__MACH__)
		#include <TargetConditionals.h>
		/* TARGET_OS_MAC exists on all the platforms
		 * so we must check all of them (in this order)
		 * to ensure that we're running on MAC
		 * and not some other Apple platform */
		#if TARGET_IPHONE_SIMULATOR == 1
			#error "IOS simulator is not supported!"
		#elif TARGET_OS_IPHONE == 1
			#define PR_PLATFORM_IOS
			#error "IOS is not supported!"
		#elif TARGET_OS_MAC == 1
			#define PR_PLATFORM_MACOS
			#error "MacOS is not supported!"
		#else
			#error "Unknown Apple platform!"
		#endif
	/* We also have to check __ANDROID__ before __linux__
	* since android is based on the linux kernel
	* it has __linux__ defined */
	#elif defined(__ANDROID__)
		#define PR_PLATFORM_ANDROID
		#error "Android is not supported!"
	#elif defined(__linux__)
		#define PR_PLATFORM_LINUX
		#error "Linux is not supported!"
	#else
		/* Unknown compiler/platform */
		#error "Unknown platform!"
	#endif
#endif

#if PR_ENABLE_ASSERTS
	#define PR_CORE_ASSERT(x, ...) {if(!(x)) { PR_CORE_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak();}}
	#define PR_ASSERT(x, ...) {if(!(x)) { PR_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak();}}
#else
	#define PR_CORE_ASSERT(x, ...)
	#define PR_ASSERT(x, ...)
#endif

#ifdef PR_ENABLE_RENDERER_LOG_LEVEL 
	#define PR_ENABLE_RENDERER_LOG 
#endif 
#if PR_ENABLE_RENDERER_LOG 
	#ifndef PR_ENABLE_CORE_LOG 
		// We do require the core logger
		#define PR_ENABLE_CORE_LOG 
	#endif 
	#ifndef PR_ENABLE_RENDERER_LOG_LEVEL 
		// Default: warning and above 
		#define PR_ENABLE_RENDERER_LOG_LEVEL 2
	#endif 
#endif

#define PR_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define BIT(x) (1 << x)



namespace Pearly {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}


	using uint64 = uint64_t;
	using uint32 = uint32_t;
	using uint16 = uint16_t;
	using uint8 = uint8_t;
}
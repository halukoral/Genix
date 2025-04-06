#pragma once

#include <memory>

#include "Genix/Core/PlatformDetection.h"

#ifdef GX_DEBUG
	#if defined(GX_PLATFORM_WINDOWS)
		#define GX_DEBUGBREAK() __debugbreak()
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define GX_ENABLE_ASSERTS
#else
	#define GX_DEBUGBREAK()
#endif

#define GX_EXPAND_MACRO(x) x
#define GX_STRINGIFY_MACRO(x) #x

#ifdef GX_ENABLE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define GX_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { LOG##type##ERROR(msg, __VA_ARGS__); GX_DEBUGBREAK(); } }
	#define GX_INTERNAL_ASSERT_WITH_MSG(type, check, ...) GX_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define GX_INTERNAL_ASSERT_NO_MSG(type, check) GX_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", GX_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define GX_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define GX_INTERNAL_ASSERT_GET_MACRO(...) GX_EXPAND_MACRO( GX_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, GX_INTERNAL_ASSERT_WITH_MSG, GX_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define ASSERT(...) GX_EXPAND_MACRO( GX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define ASSERT_CORE(...) GX_EXPAND_MACRO( GX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define ASSERT(...)
	#define ASSERT_CORE(...)
#endif

#define BIT(x) (1 << x)

#define GX_BIND(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


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

template<typename T>
using Weak = std::weak_ptr<T>;

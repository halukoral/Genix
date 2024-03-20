#pragma once

#include <memory>

#include "Genix/PlatformDetection.h"

#ifdef GX_DEBUG
	#define GX_ENABLE_ASSERTS
#endif

#ifdef GX_ENABLE_ASSERTS
	#define ASSERT(x, ...)		{ if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define ASSERT_CORE(x, ...) { if(!(x)) { LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ASSERT(x, ...)
	#define ASSERT_CORE(x, ...)
#endif

#define BIT(x) (1 << x)

#define GX_BIND(fn) std::bind(&fn, this, std::placeholders::_1)

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

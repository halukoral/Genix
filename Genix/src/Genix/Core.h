#pragma once

#if GX_DYNAMIC_LINK
	#ifdef GX_BUILD_DLL
		#define GENIX_API __declspec(dllexport)
	#else
		#define GENIX_API __declspec(dllimport)
	#endif
#else
	#define GENIX_API
#endif

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

template<typename T>
using Ref = std::shared_ptr<T>;

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
	#define GX_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define GX_CORE_ASSERT(x, ...) { if(!(x)) { LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define GX_ASSERT(x, ...)
	#define GX_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define GX_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
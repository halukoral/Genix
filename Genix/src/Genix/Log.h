#pragma once
#include "Core.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

class Log
{
public:
	static void Init();

	static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
	static Ref<spdlog::logger> s_CoreLogger;
	static Ref<spdlog::logger> s_ClientLogger;
};

#define LOG_CORE_TRACE(...)		Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)		Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)		Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...)		Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...)	Log::GetCoreLogger()->critical(__VA_ARGS__)

#define LOG_TRACE(...)			Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)			Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)			Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)			Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)		Log::GetClientLogger()->critical(__VA_ARGS__)

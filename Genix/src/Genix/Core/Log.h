#pragma once
#include "Genix/Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

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

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

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

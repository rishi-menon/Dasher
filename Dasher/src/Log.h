#pragma once
#include <memory>
#include <spdlog/spdlog.h>

namespace CoreLogger
{
	
	extern void Init();

	//GL logging/debugging support
	extern void glClearError();
	extern void glCheckError(int line = 0, const char* const functionDesc = nullptr);

	extern std::shared_ptr<spdlog::logger> g_CoreLogger;
}

#define LOG_TRACE(...)	::CoreLogger::g_CoreLogger->trace (__VA_ARGS__)
#define LOG_INFO(...)	::CoreLogger::g_CoreLogger->info (__VA_ARGS__)
#define LOG_WARN(...)	::CoreLogger::g_CoreLogger->warn (__VA_ARGS__)
#define LOG_ERROR(...)	::CoreLogger::g_CoreLogger->error (__VA_ARGS__)
#define LOG_FATAL(...)	::CoreLogger::g_CoreLogger->fatal (__VA_ARGS__)

#define ASSERT(x, ...)		if (!(x)) {LOG_WARN(__VA_ARGS__); __debugbreak();}

#define glcall(x)		::CoreLogger::glClearError (); \
						x; \
						::CoreLogger::glCheckError( __LINE__, #x);
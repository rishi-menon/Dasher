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
	extern std::shared_ptr<spdlog::logger> g_ClientLogger;
}

#define LOG_CLIENT_TRACE(...)	::CoreLogger::g_ClientLogger->trace (__VA_ARGS__)
#define LOG_CLIENT_INFO(...)	::CoreLogger::g_ClientLogger->info (__VA_ARGS__)
#define LOG_CLIENT_WARN(...)	::CoreLogger::g_ClientLogger->warn (__VA_ARGS__)
#define LOG_CLIENT_ERROR(...)	::CoreLogger::g_ClientLogger->error (__VA_ARGS__)
#define LOG_CLIENT_FATAL(...)	::CoreLogger::g_ClientLogger->fatal (__VA_ARGS__)


#ifdef _DEBUG
#define LOG_TRACE(...)	::CoreLogger::g_CoreLogger->trace (__VA_ARGS__)
#define LOG_INFO(...)	::CoreLogger::g_CoreLogger->info (__VA_ARGS__)
#define LOG_WARN(...)	::CoreLogger::g_CoreLogger->warn (__VA_ARGS__)
#define LOG_ERROR(...)	::CoreLogger::g_CoreLogger->error (__VA_ARGS__)
#define LOG_FATAL(...)	::CoreLogger::g_CoreLogger->fatal (__VA_ARGS__)

#define ASSERT(x, ...)		if (!(x)) {LOG_WARN(__VA_ARGS__); __debugbreak();}

#define glcall(x)		::CoreLogger::glClearError (); \
						x; \
						::CoreLogger::glCheckError( __LINE__, #x);

#else
#define LOG_TRACE(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)
#define ASSERT(x, ...)
#define glcall(x)

#endif

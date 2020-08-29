#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

#include <gl/glew.h>

namespace CoreLogger
{
	std::shared_ptr<spdlog::logger> g_CoreLogger;
	std::shared_ptr<spdlog::logger> g_ClientLogger;

	void Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		g_CoreLogger = spdlog::stdout_color_mt("Core");
		g_CoreLogger->set_level(spdlog::level::trace);
		
		g_ClientLogger = spdlog::stdout_color_mt("Logger");
		g_ClientLogger->set_level(spdlog::level::trace);

		LOG_TRACE("Initialised logging");
	}
	extern void glClearError()
	{
		int i = 0;
		for (; glGetError() && i < 30; i++);
		ASSERT(i < 30, "Infinite loop while checking for OpenGl errors");
	}

	extern void glCheckError(int line, const char* const functionDesc)
	{
		bool bShouldAssert = false;
		unsigned int error = glGetError();
		while (error)
		{
			bShouldAssert = true;
			error = glGetError();
			LOG_WARN ("OpenGl Error {0}: {1} AT LINE {2}", error, line, functionDesc);
		}

		ASSERT(!bShouldAssert, "OpenGl Error occurred");
	}
}
#include "Application/Application.h"
#include "Log.h"
#include "Maths.h"
#include "Constants.h"

#include "CommandLineFeatures.h"

int main(int argc, const char* argv[])
{
	g_argc = argc;
	g_argv = argv;
	CoreLogger::Init();
	Random::Init();

	if (CommandLineFeatures::ParseCommandLineBeforeWindow(g_argc, g_argv))
	{
		return 0;
	}
	
	LOG_TRACE("Started Application");

	glfwSetErrorCallback([](int error, const char* const desc)
	{
		ASSERT(false, "GLFW Error {0}: {1}", error, desc);
	});

	if (!glfwInit())
	{
		ASSERT(false, "Could not initialise GLFW");
		return 0;
	}

#if 1
	const GLFWvidmode* modes = glfwGetVideoMode(glfwGetPrimaryMonitor());
	const int nStartingWidth = static_cast<int>(modes->width / 1.2);
	const int nStartingHeight = static_cast<int>(modes->height / 1.2);
#else
	const int nStartingWidth = 1600, nStartingHeight = 1200;
#endif

	Application* pApplication = new Application;
	if (pApplication->Initialise(nStartingWidth, nStartingHeight, "Phase"))
	{
		pApplication->Run();
	}

	LOG_TRACE("Terminating Application");
	delete pApplication;
	glfwTerminate();
	return 0;
}
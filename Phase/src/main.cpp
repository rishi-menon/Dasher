#include "Application/Application.h"
#include "Log.h"
#include "Maths.h"
#include "Constants.h"

#include "AssetManagement/Texture.h"
#include "CommandLineFeatures.h"

int main(int argc, const char* argv[])
{
	CoreLogger::Init();
	Random::Init();

	if (CommandLineFeatures::ParseCommandLine(argc, argv))
	{
		return 0;
	}
	
	LOG_CLIENT_TRACE("Started Application");

	

#ifdef RM_MAC
	g_strCurrentDirectory = CommandLineFeatures::GetMyCurrentDirectory(argv[0]);
	LOG_INFO ("Current Directory: {0}", g_strCurrentDirectory.c_str());
#endif

	glfwSetErrorCallback([](int error, const char* const desc)
	{
		ASSERT(false, "GLFW Error {0}: {1}", error, desc);
	});

	if (!glfwInit())
	{
		ASSERT(false, "Could not initialise GLFW");
		return 0;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef RM_MAC
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef RM_WINDOW_NO_RESIZE
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#endif

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

	LOG_CLIENT_TRACE("Terminating Application");
	delete pApplication;
	glfwTerminate();
	return 0;
}
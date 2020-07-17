#include "Application/Application.h"
#include "Log.h"

int main()
{
	CoreLogger::Init();

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
	
	Application* pApplication = new Application;
	const int nStartingWidth = 1600, nStartingHeight = 1200;

	if (pApplication->Initialise(nStartingWidth, nStartingHeight, "Dasher"))
	{
		pApplication->Run();
	}

	delete pApplication;
	glfwTerminate();
	return 0;
}
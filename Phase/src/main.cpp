#include "Application/Application.h"
#include "Log.h"
#include "Maths.h"
#include "Constants.h"

#include "AssetManagement/Texture.h"
#include "Renderer/RendererUtils.h"

std::string GetMyCurrentDirectory(const char* argv1)
{
	std::string path;
	path.reserve (100);

	int len = mystrlen (argv1);
	int i = len-1;

	for (; i >= 0; i--)
	{
		if (argv1[i] == '/' || argv1[i] == '\\')
		{
			break;
		}
	}
	for (int j = 0; j <= i; j++)
	{
		path.push_back (argv1[j]);
	}

	return path;
}

int main(int argc, const char* argv[])
{
	CoreLogger::Init();
	Random::Init();
	LOG_CLIENT_TRACE("Started Application");

#ifdef RM_MAC
	g_strCurrentDirectory = GetMyCurrentDirectory(argv[0]);
	LOG_CLIENT_INFO ("Current Directory: {0}", g_strCurrentDirectory.c_str());
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

	Application* pApplication = new Application;
	const int nStartingWidth = 1600, nStartingHeight = 1200;

	if (pApplication->Initialise(nStartingWidth, nStartingHeight, "Phaze"))
	{
		pApplication->Run();
	}

	LOG_CLIENT_TRACE("Terminating Application");
	delete pApplication;
	glfwTerminate();
	return 0;
}
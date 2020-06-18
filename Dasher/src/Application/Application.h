#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Log.h"

class Application
{
public:
	Application();

	inline static Application* GetCurrentApp() { ASSERT(ms_currentApp, "Application was nullptr"); return ms_currentApp; }

	bool Initialise(int nWidth, int nHeight, const char* const strTitle);
	void Run();

private:
	void Cleanup();

private:
	static Application* ms_currentApp;
	static double ms_dCurrentTime;
	static double ms_dDeltaTime;
private:
	GLFWwindow* m_pWindow;

	
};
#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Log.h"
#include "Events/Layer.h"

#include <vector>

class Application
{
public:
	Application();

	inline static Application* GetCurrentApp() { ASSERT(ms_currentApp, "Application was nullptr"); return ms_currentApp; }

	inline const std::vector<Layer*>& GetLayers() const { return m_vLayers; }

	inline int GetWidth() const { return m_nWidth; }
	inline int GetHeight() const { return m_nHeight; }
	
	inline void SetWidth(int nWidth)   { m_nWidth = nWidth; }
	inline void SetHeight(int nHeight) { m_nHeight = nHeight; }

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
	int m_nWidth, m_nHeight;

	std::vector<Layer*> m_vLayers;
	
};

#include "Application.h"

#include <thread>
#include <chrono>

#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

#include "Log.h"
#include "Timer.h"

#include "Events/Layer.h"

Application* Application::ms_currentApp = nullptr;


Application::Application() :
	m_pWindow(nullptr)
{
	ASSERT(!ms_currentApp, "A second Application was created");
	ms_currentApp = this;
}

bool Application::Initialise(int nWidth, int nHeight, const char* const strTitle)
{

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_pWindow = glfwCreateWindow(nWidth, nHeight, strTitle, nullptr, nullptr);
	if (!m_pWindow)
	{
		ASSERT(false, "Could not create a window");
		return false;
	}

	glfwMakeContextCurrent(m_pWindow);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		ASSERT(false, "Could not initialise GLEW");
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//										Set Callbacks											 //
	///////////////////////////////////////////////////////////////////////////////////////////////////
	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int nWidth, int nHeight)
		{
			//LOG_INFO("Window Resize Event: width: {0} height: {1}", nWidth, nHeight);
			Application* pApp = (Application*)glfwGetWindowUserPointer(pWindow);
			ASSERT(pApp, "pApplication was nullptr");
			
			pApp->SetWidth(nWidth);
			pApp->SetHeight(nHeight);

			Renderer::OnWindowResize(nWidth, nHeight);
			const std::vector<Layer*>& vec = pApp->GetLayers();
			for (Layer* pLayer : vec)
			{
				if (pLayer->OnWindowResize(nWidth, nHeight))
				{
					break;
				}

			}
		});

	glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			//LOG_INFO("Mouse Press Event: {0}, {1}, {2}", button, action, mods);
			Application* pApp = (Application*)glfwGetWindowUserPointer(window);
			ASSERT(pApp, "pApplication was nullptr");

			const std::vector<Layer*>& vec = pApp->GetLayers();
			switch (action)
			{
				case GLFW_PRESS:
				{
					for (Layer* pLayer : vec)
					{
						if (pLayer->OnMouseDown(button)) break;
					}
				}
				case GLFW_RELEASE:
				{
					for (Layer* pLayer : vec)
					{
						if (pLayer->OnMouseUp(button)) break;
					}
				}
			}

		});

	glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* window, double xpos, double ypos)
		{
			//LOG_INFO("Mouse Move Event: {0}, {1}", xpos, ypos);
			Application* pApp = (Application*)glfwGetWindowUserPointer(window);
			ASSERT(pApp, "pApplication was nullptr");

			const std::vector<Layer*>& vec = pApp->GetLayers();
			for (Layer* pLayer : vec)
			{
				if (pLayer->OnMouseMove(xpos, ypos)) break;
			}
		});

	glfwSetKeyCallback(m_pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			//LOG_INFO("Key Press Event: {0}, {1}, {2}, {3}", key, scancode, action, mods);
			Application* pApp = (Application*)glfwGetWindowUserPointer(window);
			ASSERT(pApp, "pApplication was nullptr");

			const std::vector<Layer*>& vec = pApp->GetLayers();
			switch (action)
			{
				case GLFW_PRESS:
				{
					for (Layer* pLayer : vec)
					{
						if (pLayer->OnKeyDown(key)) break;
					}
				}
				case GLFW_RELEASE:
				{
					for (Layer* pLayer : vec)
					{
						if (pLayer->OnKeyUp(key)) break;
					}
				}
				case GLFW_REPEAT:
				{
					for (Layer* pLayer : vec)
					{
						if (pLayer->OnKey(key)) break;
					}
				}
			}
		});

	//Enable blending and depth buffer
	glcall(glEnable(GL_BLEND));
	glcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	bool bSuccess = Renderer::Initialise();
	ASSERT(bSuccess, "Error: Failed to initialise renderer");

	return bSuccess;
}
void Application::Run()
{
	const float fCol = 0.18f;
	glClearColor(fCol, fCol, fCol, 1);

	//Nice Quad Colour
/*
	QuadShape shape;
	shape[0] = Vertex({ -0.5, -0.5, 0 }, { 0.3, 0.2, 0.2, 1.0 }, { 0.0, 0.0 });
	shape[1] = Vertex({  0.5, -0.5, 0 }, { 0.9, 0.2, 0.2, 1.0 }, { 0.0, 0.0 });
	shape[2] = Vertex({  0.5,  0.5, 0 }, { 0.7, 0.2, 0.2, 1.0 }, { 0.0, 0.0 });
	shape[3] = Vertex({ -0.5,  0.5, 0 }, { 0.9, 0.2, 0.2, 1.0 }, { 0.0, 0.0 });
	shape[3] = Vertex({ -0.5,  0.5, 0 }, { 0.9, 0.2, 0.2, 1.0 }, { 0.0, 0.0 });
*/
	
	/*
	QuadShape shape2;

	shape2[0] = Vertex({ 0.7, 0.7, 0 },		{ 0.1, 0.2, 0.2, 1.0 },		{ 0.0, 0.0 });
	shape2[1] = Vertex({ 1, 0.7, 0 },		{ 0.5, 0.2, 0.2, 1.0 },		{ 0.0, 1.0 });
	shape2[2] = Vertex({ 1, 1, 0 },			{ 0.7, 0.2, 0.2, 1.0 },		{ 1.0, 1.0 });
	shape2[3] = Vertex({ 0.7,  1, 0 },		{ 0.9, 0.2, 0.2, 1.0 },		{ 1.0, 0.0 });*/

	unsigned int idTemp = Texture::LoadTexture("Assets\\Textures\\img1.jpg", 0, 0);


	const double dMaxDeltaTime = 1.0/30.0;
	const double dTargetDeltaTime = 1.0 / 60.0;

	

	while (!glfwWindowShouldClose (m_pWindow))
	{

		///////////////////////////////////////////////////////////
		//////                  Set Time               ////////////
		double dCurrentTime = glfwGetTime();
		double dDeltaTime = dCurrentTime - timer.GetGameTime();
		if (dDeltaTime > dMaxDeltaTime)
		{
			LOG_INFO("Frame rate is slowing down: {0} ms", dDeltaTime * 1000);
			dDeltaTime = dMaxDeltaTime;
		}
		timer.SetDeltaTime(dDeltaTime);
		timer.SetGameTime(dCurrentTime);

		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

		//clear screen
		glcall(glClear (GL_COLOR_BUFFER_BIT));


		//Renderer::DrawQuad({ 0,0 }, { 0.5, 0.5 }, { 0.9,0.7,0.1,1.0 });
		Renderer::DrawQuad({ 100, 100 }, { 100,100 }, { 0.3,0.2,0.8,1.0 }, idTemp);
		Renderer::DrawQuad({ 200, 200 }, { 100,100 }, { 0.1,0.5,0.8,1.0 }, idTemp);
		Renderer::DrawQuad({ 300, 300 }, { 100,100 }, { 0.8,0.2,0.8,1.0 }, idTemp);

		/*RendererVertex v[6];
		unsigned int i[15] = { 0,1,2,0,2,3,0,3,4,0,4,5,0,5,1 };	
		
		v[0].SetPosColTex({ 0,0, 0 }, { 0.8,0.2,0.1, 1.0 }			,{0,0});
		v[1].SetPosColTex({ 0,0.3, 0 }, { 0.8,0.2,0.1, 1.0 }		,{0,1});
		v[2].SetPosColTex({ 0.2,0.1, 0 }, { 0.8,0.2,0.1, 1.0 }		,{1,1});
		v[4].SetPosColTex({ 0.2,-0.1, 0 }, { 0.8,0.2,0.1, 1.0 }		,{1,0});
		v[5].SetPosColTex({ -0.2,-0.1, 0 }, { 0.8,0.2,0.1, 1.0 }	,{0,0});
		v[3].SetPosColTex({ -0.2,0.1, 0 }, { 0.8,0.2,0.1, 1.0 }		,{0,0});
		
		Renderer::DrawQuadTexture(v, 6, i, 15, idTemp);*/

		Renderer::Flush();

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();


		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		///////////////////////////////////////////////////////////
		//////                  Sleep                  ////////////
		long dSleepTime = 1; // in ms
		if (dDeltaTime < dTargetDeltaTime)
		{
			double extraSleep = (dTargetDeltaTime - dDeltaTime) * 1000;
			dSleepTime += (long) extraSleep;
		}
		std::chrono::microseconds sleepDuration(static_cast<long>(dSleepTime));
		std::this_thread::sleep_for(sleepDuration);

	}

	Cleanup();
}
void Application::Cleanup()
{
	Renderer::Cleanup();
	if (m_pWindow)
	{
		glfwDestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}
}


#include "Application.h"

#include <thread>
#include <chrono>

#include "Renderer/Renderer.h"
#include "AllShapes.h"

#include "Log.h"
#include "Timer.h"

Application* Application::ms_currentApp = nullptr;


Application::Application() :
	m_pWindow(nullptr)
{
	ASSERT(!ms_currentApp, "A second Application was created");
	ms_currentApp = this;
}

bool Application::Initialise(int nWidth, int nHeight, const char* const strTitle)
{
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

	//Enable blending and depth buffer

	glcall(glEnable(GL_BLEND));
	glcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	bool bSucecss = Renderer::Initialise();
	ASSERT(bSucecss, "Error: Failed to initialise renderer");

	return bSucecss;
}
void Application::Run()
{
	const float fCol = 0.18;
	glClearColor(fCol, fCol, fCol, 1);

	//Nice Quad Colour
/*
	QuadShape shape;
	shape[0] = Vertex({ -0.5, -0.5, 0 }, { 0.3, 0.2, 0.2, 1.0 }, { 0.0, 0.0 });
	shape[1] = Vertex({  0.5, -0.5, 0 }, { 0.9, 0.2, 0.2, 1.0 }, { 0.0, 0.0 });
	shape[2] = Vertex({  0.5,  0.5, 0 }, { 0.7, 0.2, 0.2, 1.0 }, { 0.0, 0.0 });
	shape[3] = Vertex({ -0.5,  0.5, 0 }, { 0.9, 0.2, 0.2, 1.0 }, { 0.0, 0.0 });
*/
	QuadShape shape;

	shape[0] = Vertex({ -0.5, -0.5, 0 }, { 0.3, 0.2, 0.2, 1.0 }, { 0.0, 0.0 });
	shape[1] = Vertex({  0.5, -0.5, 0 }, { 0.9, 0.2, 0.2, 1.0 }, { 1.0, 0.0 });
	shape[2] = Vertex({  0.5,  0.5, 0 }, { 0.7, 0.2, 0.2, 1.0 }, { 1.0, 1.0 });
	shape[3] = Vertex({ -0.5,  0.5, 0 }, { 0.9, 0.2, 0.2, 1.0 }, { 0.0, 1.0 });

	Texture tex;
	tex.LoadTexture("Assets\\Textures\\img1.jpg");
	QuadShape shape1;

	shape1[0] = Vertex({ -1, -1, 0 },		{ 0.1, 0.2, 0.2, 1.0 },		{ 0.0, 0.0 });
	shape1[1] = Vertex({ -0.7, -1, 0 },		{ 0.5, 0.2, 0.2, 1.0 },		{ 0.0, 0.0 });
	shape1[2] = Vertex({ -0.7,  -0.7, 0 },	{0.7, 0.2, 0.2, 1.0 },		{ 0.0, 0.0 });
	shape1[3] = Vertex({ -1,  -0.7, 0 },	{ 0.9, 0.2, 0.2, 1.0 },		{ 0.0, 0.0 });
	/*
	QuadShape shape2;

	shape2[0] = Vertex({ 0.7, 0.7, 0 },		{ 0.1, 0.2, 0.2, 1.0 },		{ 0.0, 0.0 });
	shape2[1] = Vertex({ 1, 0.7, 0 },		{ 0.5, 0.2, 0.2, 1.0 },		{ 0.0, 1.0 });
	shape2[2] = Vertex({ 1, 1, 0 },			{ 0.7, 0.2, 0.2, 1.0 },		{ 1.0, 1.0 });
	shape2[3] = Vertex({ 0.7,  1, 0 },		{ 0.9, 0.2, 0.2, 1.0 },		{ 1.0, 0.0 });*/


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
		
		//clear screen
		glcall(glClear (GL_COLOR_BUFFER_BIT));

		Renderer::DrawShape(shape, tex);
		Renderer::DrawShape(shape1);
		//Renderer::DrawShape(shape2);

		Renderer::EndScene();
		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();

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

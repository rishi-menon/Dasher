
#include "Application.h"

#include <thread>
#include <chrono>

#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

#include "Log.h"
#include "Timer.h"

#include "EventCallbacks.h"

#include "Events/Layer.h"
#include "Events/PlayerLayer.h"

Application* Application::ms_currentApp = nullptr;


Application::Application() :
	m_pWindow(nullptr)
{
	ASSERT(!ms_currentApp, "A second Application was created");
	ms_currentApp = this;
}

bool Application::Initialise(int nWidth, int nHeight, const char* const strTitle)
{
	m_vLayers.reserve(30);
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

	//Set callbacks
	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetWindowSizeCallback(m_pWindow, WindowResizeCallback);
	glfwSetMouseButtonCallback(m_pWindow, MouseButtonCallback);
	glfwSetCursorPosCallback(m_pWindow, MousePositionCallback);
	glfwSetKeyCallback(m_pWindow, KeyCallback);

	//Enable blending and depth buffer
	glcall(glEnable(GL_BLEND));
	glcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	bool bSuccess = Renderer::Initialise();
	ASSERT(bSuccess, "Error: Failed to initialise renderer");

	///////////////////////////////////////////////////////////////////////////////
	//								Create Layers here							 //
	///////////////////////////////////////////////////////////////////////////////
	
	InsertLayer(new PlayerLayer);
	return bSuccess;
}


void Application::InsertLayer(Layer* pLayer)
{
	std::vector<Layer*>::iterator it = std::find(m_vLayers.begin(), m_vLayers.end(), pLayer);
	if (it != m_vLayers.end())
	{
		int index = it - m_vLayers.begin();
		ASSERT(false, "Layer is already inserted");
	}
	else
	{
		int index = m_vLayers.size();
		m_vLayers.push_back (pLayer);
		pLayer->RegisterEvents(this, index);
	}

	//Call OnStart for the inserted layer
	pLayer->OnStart();
}
void Application::RegisterEvents(Layers layerId, LayerIndex index)
{
	std::list <LayerIndex>& list = m_listLayersIndex[layerId];

	std::list <LayerIndex>::iterator it = std::find(list.begin(), list.end(), index);
	if (it != list.end())
	{
		LOG_WARN("Layer {0} with index {1} was already registered", layerId, index);
	}
	else
	{
		list.push_back(index);
	}
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
		double dDeltaTime = dCurrentTime - timer.GetGameTime();	//in seconds
		if (dDeltaTime > dMaxDeltaTime)
		{
			//LOG_INFO("Frame rate is slowing down: {0} ms", dDeltaTime * 1000);
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

		for (Layer* pLayer : m_vLayers)
		{
			pLayer->OnUpdate(dDeltaTime);
		}
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

	std::vector <Layer*>& vec = m_vLayers;
	for (Layer* pLayer : vec)
	{
		delete pLayer;
	}
	m_vLayers.clear();
}

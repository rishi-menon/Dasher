
#include "Application.h"

#include <thread>
#include <chrono>

#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

#include "Log.h"

#include "EventCallbacks.h"

#include "Events/Layer.h"
#include "Events/PlayerLayer.h"
#include "Events/BackgroundLayer.h"
#include "Events/BlockSpawnerLayer.h"

Application* Application::ms_currentApp = nullptr;

bool s_bResetGame = false;

Application::Application() :
	m_pWindow(nullptr),
	m_dCurrentTime (0),
	m_dDeltaTime (0)
{
	ASSERT(!ms_currentApp, "A second Application was created");
	ms_currentApp = this;
}

bool Application::Initialise(int nWidth, int nHeight, const char* const strTitle)
{
	m_vLayers.reserve(10);
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
	
	InsertLayer(new BackgroundLayer);
	InsertLayer(new PlayerLayer);
	InsertLayer(new BlockSpawnerLayer);
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

void Application::ResetGame()
{
	s_bResetGame = true;
}
void Application::StartGame()
{
	s_bResetGame = false;
	Renderer::Flush();
	glcall(glClear(GL_COLOR_BUFFER_BIT));
	for (Layer* pLayer : m_vLayers)
	{
		pLayer->ResetLayer();
	}
}

void Application::Run()
{
	const float fCol = 0.18f;
	glClearColor(fCol, fCol, fCol, 1);

	const double dMaxDeltaTime = 1.0/30.0;
	const double dTargetDeltaTime = 1.0 / 60.0;

	//unsigned int nid = Texture::LoadTexture("Assets\\Textures\\img1.jpg", nullptr, nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));


	while (!glfwWindowShouldClose (m_pWindow))
	{
		double dCurrentTime = glfwGetTime();
		m_dDeltaTime = dCurrentTime - m_dCurrentTime;	//in seconds
		m_dCurrentTime = dCurrentTime;
		if (m_dDeltaTime > dMaxDeltaTime)
		{
			m_dDeltaTime = dMaxDeltaTime;
		}

		//clear screen
		glcall(glClear(GL_COLOR_BUFFER_BIT));

		for (Layer* pLayer : m_vLayers)
		{
			pLayer->OnUpdate(m_dDeltaTime);
			if (s_bResetGame)
			{
				StartGame();
				break;
			}
		}

		/*RendererVertex v[4];

		glm::vec4 col = { 0.8, 0.8, 0.8, 1.0 };
		v[0].SetPosColTex({ 200, 200, 0 }, col, { -1.0, -1.0 });
		v[1].SetPosColTex({ 400, 200, 0 }, col, { 1.0, -1.0 });
		v[2].SetPosColTex({ 400, 400, 0 }, col, { 1.0, 1.0 });
		v[3].SetPosColTex({ 200, 400, 0 }, col, { -1.0, 1.0 });*/

		/*unsigned int i[6] = { 0,1,2,2,3,0 };
		Renderer::DrawQuadTexture(v, 4, i, 6, nid);*/
		
		Renderer::Flush();

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents(); 

		
		if (m_dDeltaTime < 1.0 / 70.0)
		{
			std::chrono::milliseconds sleepDuration(1);
			std::this_thread::sleep_for(sleepDuration);
		}
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

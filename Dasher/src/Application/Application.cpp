
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
int Application::m_nWidth = 0;
int Application::m_nHeight = 0;

Application::Application() :
	m_pWindow(nullptr),
	m_dCurrentTime (0),
	m_dDeltaTime (0),
	m_CurMenu (Menu::MainMenu),
	m_NextMenu (Menu::MainMenu)

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

	m_NextMenu = Menu::MainMenu;
	ChangeMenuState();

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
			pLayer->OnUpdate((float)m_dDeltaTime);
		}

		//Change the menu mode if applicable
		if (m_CurMenu != m_NextMenu) { ChangeMenuState(); }

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
	ClearLayers();
}


void Application::ClearLayers()
{
	std::vector <Layer*>& vec = m_vLayers;
	for (Layer* pLayer : vec)
	{
		delete pLayer;
	}
	m_vLayers.clear();

	for (unsigned int i = 0; i < LayerCount; i++)
	{
		m_listLayersIndex[i].clear();
	}
}
void Application::OnStart()
{
	//call OnStart for each layer once all the layes have been added
	for (Layer* pLayer : m_vLayers)
	{
		pLayer->OnStart();
	}
}

void Application::ChangeMenuState()
{
	m_CurMenu = m_NextMenu;
	switch (m_NextMenu)
	{
		case Menu::MainMenu:
		{
			StartMenuMainMenu();
			break;
		}
		case Menu::NormalMode:
		{
			StartMenuNormalMode();
			break;
		}
		case Menu::PracticeMode:
		{
			StartMenuPracticeMode();
			break;
		}
		case Menu::TutorialMode:
		{
			StartMenuTutorialMode();
			break;
		}
		case Menu::Credits:
		{
			StartMenuCredits();
			break;
		}
	}
}

//Menus
void Application::StartMenuMainMenu()
{
	ClearLayers();
	InsertLayer(new BackgroundLayer(BackgroundLayerProps("Assets\\Textures\\External\\bg2.png")));
	OnStart();
}
void Application::StartMenuNormalMode()
{
	ClearLayers();
	InsertLayer(new BackgroundLayer(BackgroundLayerProps("Assets\\Textures\\External\\bg3.png")));
	InsertLayer(new PlayerLayer);
	InsertLayer(new BlockSpawnerLayer);
	OnStart();
}
void  Application::StartMenuPracticeMode()
{

}
void  Application::StartMenuTutorialMode()
{

}
void  Application::StartMenuCredits()
{

}
#include "Application.h"

#include <thread>
#include <chrono>

#include "AssetManagement/Texture.h"
#include "AssetManagement/Font.h"
#include "AssetManagement/StandardTexture.h"

#include "Renderer/Renderer.h"
#include "Log.h"

#include "EventCallbacks.h"
#include "Events/Layer.h"
#include "Events/Player/ZenPlayerLayer.h"
#include "Events/Player/NormalPlayerLayer.h"
#include "Events/Player/NormalPlayerLayerUI.h"
#include "Events/BackgroundLayer.h"
#include "Events/BlockSpawnerLayer.h"
#include "Events/UILayer.h"
#include "Events/MainMenu/MainMenuLayer.h"
#include "Events/CreditLayer.h"
#include "Events/FadeoutScreenLayer.h"

#include "Events/Tutorial/TutorialPlayerLayer.h"
#include "Events/Tutorial/TutorialBlockSpawnerLayer.h"

#include "CommandLineFeatures.h"
#include "Constants.h"

Application* Application::ms_currentApp = nullptr;
int Application::m_nWidth = 0;
int Application::m_nHeight = 0;

bool g_bShowTrajectoryEnabled = false;

Application::Application() :
	m_pWindow(nullptr),
	m_dCurrentTime (0),
	m_dDeltaTime (0),
	m_dGameLastSleepTime (0),

	m_CurMenu (Menu::MainMenu),
	m_NextMenu (Menu::MainMenu),
	m_NextMenuUserData (nullptr)

{
	ASSERT(!ms_currentApp, "A second Application was created");
	ms_currentApp = this;
}

bool Application::Initialise(int nWidth, int nHeight, const char* const strTitle)
{
	m_vLayers.reserve(10);
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef RM_OPENGL_CORE_PROFILE
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef RM_WINDOW_NO_RESIZE
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#endif

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
		return false;
	}

	if (CommandLineFeatures::ParseCommandLineAfterGlew(g_argc, g_argv))
	{
		return false;
	}

	if (!FontInit()) { return false; }
	if (!UI::UIInit()) { return false; }
	if (!StandardTextureInit()) { return false; }

	//Set callbacks
	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetWindowSizeCallback(m_pWindow, WindowResizeCallback);
	glfwSetMouseButtonCallback(m_pWindow, MouseButtonCallback);
	glfwSetCursorPosCallback(m_pWindow, MousePositionCallback);
	glfwSetKeyCallback(m_pWindow, KeyCallback);

	//Enable blending and depth buffer
	glcall(glEnable(GL_BLEND));
	glcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	
	glcall(glEnable(GL_DEPTH_TEST));
	glcall(glDepthFunc(GL_LEQUAL));

	if (!Renderer::Initialise())
	{
		LOG_CLIENT_ERROR("Error: Failed to initialise renderer");
		return false;
	}

	m_NextMenu = Menu::MainMenu;
	ChangeMenuState();

	return true;
}


void Application::InsertLayer(Layer* pLayer)
{
	std::vector<Layer*>::iterator it = std::find(m_vLayers.begin(), m_vLayers.end(), pLayer);
	if (it != m_vLayers.end())
	{
		int index = static_cast<int>(it - m_vLayers.begin());
		ASSERT(false, "Layer is already inserted");
	}
	else
	{
		int index = static_cast<int>(m_vLayers.size());
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
	const float fCol = 0.0f;
	glClearColor(fCol, fCol, fCol, 1);

	const double dMaxDeltaTime = 1.0/30.0;

	constexpr int nRegularSleepMils = 1;	//mils = milliseconds
	constexpr int nGameSecondsBeforeSleep = 2;
	constexpr int nGameSleepMils = 2;
	
	std::chrono::milliseconds chronoRegularSleep(nRegularSleepMils);
	std::chrono::milliseconds chronoGameSleep(nGameSleepMils);

	double dCurrentTime;

	while (!glfwWindowShouldClose(m_pWindow))
	{
		dCurrentTime = glfwGetTime();
		m_dDeltaTime = dCurrentTime - m_dCurrentTime;	//in seconds
		m_dCurrentTime = dCurrentTime;
		if (m_dDeltaTime > dMaxDeltaTime)
		{
			m_dDeltaTime = dMaxDeltaTime;
		}

		//clear screen
		glcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		//The UI layer has to be inserted at the beginning of the layer but has to be rendered at the end
		if (g_nUILayerIndex != -1)
		{
			for (int i = 0; i < g_nUILayerIndex; i++)
			{
				m_vLayers[i]->OnUpdate((float)m_dDeltaTime);
			}
			for (unsigned int i = g_nUILayerIndex + 1; i < m_vLayers.size(); i++)
			{
				m_vLayers[i]->OnUpdate((float)m_dDeltaTime);
			}
			//Update the UI layer
			m_vLayers[g_nUILayerIndex]->OnUpdate((float)m_dDeltaTime);
		}
		else
		{
			for (Layer* pLayer : m_vLayers)
			{
				pLayer->OnUpdate((float)m_dDeltaTime);
			}
		}

		//Change the menu mode if applicable
		if (m_CurMenu != m_NextMenu) { ChangeMenuState(); }

		Renderer::Flush();

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();

		//Sleep
		if (m_CurMenu == Menu::NormalMode || m_CurMenu == Menu::ZenMode || m_CurMenu == Menu::TutorialMode)
		{
			if (dCurrentTime > m_dGameLastSleepTime)
			{
				//while running the game sleep
				std::this_thread::sleep_for(chronoGameSleep);
				m_dGameLastSleepTime = dCurrentTime + nGameSecondsBeforeSleep;
			}
		}
		else
		{
			std::this_thread::sleep_for(chronoRegularSleep);
		}
	}

	Cleanup();
}
void Application::Cleanup()
{
	Renderer::Cleanup();
	FontCleanUp();
	UI::UICleanup();
	StandardTextureCleanup();

	ClearLayers();

	if (m_pWindow)
	{
		glfwDestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}
}


void Application::ClearLayers()
{
	std::vector <Layer*>& vec = m_vLayers;
	//Do not change to std::size_t as it is unsigned and the for loop breaks when the value becomes negative
	for (int i = (int)(vec.size() - 1); i >= 0; i--)
	{
		if (vec[i])
			delete vec[i];
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
//Menus
void Application::ChangeMenuState()
{
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
		case Menu::ZenMode:
		{
			StartMenuZenMode();
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
		//This is a hack, The menu will change only if the nextMenu is different than the current one. So in order to restart you could invalidate the nextMenu. To do: Add a Menu::Restart option perhaps ?
		case Menu::None:
		{
			//In this case restart the current menu (Eg: Play again button for the main menu
			LOG_INFO("Restarting Layer");
			ASSERT(m_CurMenu != Menu::None, "Recursion incoming!");
			m_NextMenu = m_CurMenu;
			ChangeMenuState();
			break;
		}
	}
	m_CurMenu = m_NextMenu;
}

void Application::StartMenuMainMenu()
{
	//To do: for future update, have a system to prevent deleting the layer instead of deleting it and then creating it again
	BackgroundLayerProps props;
	if (m_CurMenu == Menu::Credits)
	{
		for (Layer* pLayer : m_vLayers)
		{
			BackgroundLayer* pBgLayer = dynamic_cast<BackgroundLayer*> (pLayer);
			if (pBgLayer)
			{
				props = pBgLayer->GetBackgroundProp();
			}
		}
	}
	//If it was not found then itll use the default constructor
	props.textureType = StandardTexture::Background2;

	ClearLayers();
	InsertLayer(new UILayer);
	InsertLayer(new BackgroundLayer(props));
	InsertLayer(new MainMenuLayer);
	OnStart();
}
void Application::StartMenuNormalMode()
{
	ClearLayers();
	InsertLayer(new UILayer);
	InsertLayer(new BackgroundLayer(BackgroundLayerProps(StandardTexture::Background3)));
	InsertLayer(new NormalPlayerLayer);
	InsertLayer(new BlockSpawnerLayer);
	InsertLayer(new FadeoutScreenLayer);
	InsertLayer(new NormalPlayerLayerUI);

	OnStart();
}
void  Application::StartMenuZenMode()
{
	ClearLayers();
	InsertLayer(new UILayer);
	InsertLayer(new BackgroundLayer(BackgroundLayerProps(StandardTexture::Background1)));
	InsertLayer(new ZenPlayerLayer);
	InsertLayer(new BlockSpawnerLayer);
	OnStart();
}
void  Application::StartMenuTutorialMode()
{
	ClearLayers();
	InsertLayer(new UILayer);
	InsertLayer(new BackgroundLayer(BackgroundLayerProps(StandardTexture::Background4)));
	InsertLayer(new TutorialPlayerLayer);
	InsertLayer(new TutorialBlockSpawnerLayer);
	InsertLayer(new NormalPlayerLayerUI);
	OnStart();
}
void  Application::StartMenuCredits()
{
	//Find the backgroundlayer
	BackgroundLayerProps props;
	if (m_CurMenu == Menu::MainMenu)
	{
		for (Layer* pLayer : m_vLayers)
		{
			BackgroundLayer* pBgLayer = dynamic_cast<BackgroundLayer*> (pLayer);
			if (pBgLayer)
			{
				props = pBgLayer->GetBackgroundProp();
			}
		}
	}
	//If it was not found then itll use the default constructor
	props.textureType = StandardTexture::Background2;

	ClearLayers();
	InsertLayer(new UILayer);
	InsertLayer(new BackgroundLayer(props));
	InsertLayer(new CreditLayer);
	
	OnStart();
}
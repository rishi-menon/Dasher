#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Log.h"
#include "Events/Layer.h"

#include <vector>
#include <list>

extern bool g_bShowTrajectoryEnabled;

enum class Menu : unsigned int 
{
	None = 0,
	MainMenu,
	
	//Games
	NormalMode,
	ZenMode,
	TutorialMode,

	Credits
};
class Application
{
public:
	Application();

	inline static Application* GetCurrentApp() { ASSERT(ms_currentApp, "Application was nullptr"); return ms_currentApp; }
	inline static double GetGameTime() { return glfwGetTime();}

	inline const std::vector<Layer*>& GetLayers() const { return m_vLayers; }
	inline const std::list<LayerIndex>& GetLayerIndex(Layers layer) const { return m_listLayersIndex[layer]; }

	inline static int GetWidth()  { return m_nWidth; }
	inline static int GetHeight() { return m_nHeight; }
	
	inline static void SetWidth(int nWidth)   { m_nWidth = nWidth; }
	inline static void SetHeight(int nHeight) { m_nHeight = nHeight; }

	bool Initialise(int nWidth, int nHeight, const char* const strTitle);
	void Run();

	void InsertLayer(Layer* pLayer);
	void RegisterEvents(Layers layerId, LayerIndex index);

	inline Menu GetCurrentMenu() const { return m_CurMenu; }
	inline void SetNextMenu(Menu menu, void* userData) { m_NextMenu = menu; m_NextMenuUserData = userData; }

	inline void GetMousePos(double& x, double& y) const { glfwGetCursorPos(m_pWindow, &x, &y); y = m_nHeight - y; }

private:
	void Cleanup();
	void ClearLayers();
	void OnStart();

	void ChangeMenuState();
	//States
	void StartMenuMainMenu();
	void StartMenuNormalMode();
	void StartMenuZenMode();
	void StartMenuTutorialMode();
	void StartMenuCredits();

private:
	static Application* ms_currentApp;
	static int m_nWidth, m_nHeight;
	
private:
	GLFWwindow* m_pWindow;

	double m_dCurrentTime;
	double m_dDeltaTime;

	std::vector<Layer*> m_vLayers;
	//create a vector for each event callback. The vector stores the index position in the actual m_vLayers array. When the event occurs, the corresponding OnEvent function will be called for all the layers stored in the corresponding index array.
	std::list<LayerIndex> m_listLayersIndex[LayerCount];

	Menu m_CurMenu, m_NextMenu;
	void* m_NextMenuUserData;
};
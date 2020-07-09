#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Log.h"
#include "Events/Layer.h"

#include <vector>
#include <list>

class Application
{
public:
	Application();

	inline static Application* GetCurrentApp() { ASSERT(ms_currentApp, "Application was nullptr"); return ms_currentApp; }

	inline const std::vector<Layer*>& GetLayers() const { return m_vLayers; }
	inline const std::list<LayerIndex>& GetLayerIndex(Layers layer) const { return m_listLayersIndex[layer]; }

	inline int GetWidth() const { return m_nWidth; }
	inline int GetHeight() const { return m_nHeight; }
	
	inline void SetWidth(int nWidth)   { m_nWidth = nWidth; }
	inline void SetHeight(int nHeight) { m_nHeight = nHeight; }

	bool Initialise(int nWidth, int nHeight, const char* const strTitle);
	void Run();

	void InsertLayer(Layer* pLayer);
	void RegisterEvents(Layers layerId, LayerIndex index);

	void ResetGame();
private:
	void StartGame();
	void Cleanup();

private:
	static Application* ms_currentApp;
	
private:
	GLFWwindow* m_pWindow;
	int m_nWidth, m_nHeight;

	double m_dCurrentTime;
	double m_dDeltaTime;

	std::vector<Layer*> m_vLayers;
	
	//create a vector for each event callback. The vector stores the index position in the actual m_vLayers array. When the event occurs, the corresponding OnEvent function will be called for all the layers stored in the corresponding index array.
	std::list<LayerIndex> m_listLayersIndex[LayerCount];
};
#include "EventCallbacks.h"
#include "Renderer/Renderer.h"

void WindowResizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
	//LOG_INFO("Window Resize Event: width: {0} height: {1}", nWidth, nHeight);
	Application* pApp = (Application*)glfwGetWindowUserPointer(pWindow);
	ASSERT(pApp, "pApplication was nullptr");

	pApp->SetWidth(nWidth);
	pApp->SetHeight(nHeight);

	Renderer::OnWindowResize(nWidth, nHeight);
	const std::vector<Layer*>& layers = pApp->GetLayers();
	const std::list<LayerIndex>& index = pApp->GetLayerIndex(LayerWindowResize);
	for (LayerIndex i : index)
	{
		ASSERT(i < (LayerIndex)layers.size(), "Index out of bounds");
		if (layers[i]->OnWindowResize(nWidth, nHeight))
		{
			break;
		}

	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//LOG_INFO("Mouse Press Event: {0}, {1}, {2}", button, action, mods);
	Application* pApp = (Application*)glfwGetWindowUserPointer(window);
	ASSERT(pApp, "pApplication was nullptr");

	const std::vector<Layer*>& layers = pApp->GetLayers();
	switch (action)
	{
		case GLFW_PRESS:
		{
			const std::list<LayerIndex>& index = pApp->GetLayerIndex(LayerMouseDown);
			for (LayerIndex i : index)
			{
				ASSERT(i < (LayerIndex)layers.size(), "Index out of bounds");
				if (layers[i]->OnMouseDown(button)) break;
			}
			break;
		}
		case GLFW_RELEASE:
		{
			const std::list<LayerIndex>& index = pApp->GetLayerIndex(LayerMouseUp);

			for (LayerIndex i : index)
			{
				ASSERT(i < (LayerIndex)layers.size(), "Index out of bounds");
				if (layers[i]->OnMouseUp(button)) break;
			}
			break;
		}
	}
}

void MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	//LOG_INFO("Mouse Move Event: {0}, {1}", xpos, ypos);
	Application* pApp = (Application*)glfwGetWindowUserPointer(window);
	ASSERT(pApp, "pApplication was nullptr");

	const std::vector<Layer*>& layers = pApp->GetLayers();
	const std::list<LayerIndex>& index = pApp->GetLayerIndex(LayerMouseMove);

	ypos = pApp->GetHeight() - ypos;	//The origin of the game is in the bottom left corner. But the position returned by the mouse callback is with respect to the top left corner... Subtract the height to convert it to the standard coordinate system.
	for (LayerIndex i : index)
	{
		ASSERT(i < (LayerIndex)layers.size(), "Index out of bounds");
		if (layers[i]->OnMouseMove((int)xpos, (int)ypos)) break;
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//LOG_INFO("Key Press Event: {0}, {1}, {2}, {3}", key, scancode, action, mods);
	Application* pApp = (Application*)glfwGetWindowUserPointer(window);
	ASSERT(pApp, "pApplication was nullptr");

	const std::vector<Layer*>& layers = pApp->GetLayers();
	switch (action)
	{
		case GLFW_PRESS:
		{
			const std::list<LayerIndex>& index = pApp->GetLayerIndex(LayerKeyDown);

			for (LayerIndex i : index)
			{
				ASSERT(i < (LayerIndex)layers.size(), "Index out of bounds");
				if (layers[i]->OnKeyDown(key)) break;
			}
			break;
		}
		case GLFW_RELEASE:
		{
			const std::list<LayerIndex>& index = pApp->GetLayerIndex(LayerKeyUp);

			for (LayerIndex i : index)
			{
				ASSERT(i < (LayerIndex)layers.size(), "Index out of bounds");
				if (layers[i]->OnKeyUp(key)) break;
			}
			break;
		}
		case GLFW_REPEAT:
		{
			const std::list<LayerIndex>& index = pApp->GetLayerIndex(LayerKey);

			for (LayerIndex i : index)
			{
				ASSERT(i < (LayerIndex)layers.size(), "Index out of bounds");
				if (layers[i]->OnKey(key)) break;
			}
			break;
		}
	}
}
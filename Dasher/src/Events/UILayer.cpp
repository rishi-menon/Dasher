#include "UILayer.h"
#include "Application/Application.h"

UILayer* g_pCurrentUILayer = nullptr;
int g_nUILayerIndex = -1;

UILayer::UILayer()
{
	m_vObjects.reserve(10);
	
	if (g_pCurrentUILayer)
	{
		//You created two UI Layers which is a probably a mistake
		ASSERT(false, "Two UI layers was created");
	}
	g_pCurrentUILayer = this;
}

UILayer::~UILayer()
{
	//The vector stores the address of the UI objects so as to call events
	//The UILayer does not handle the allocation and deallocation of the objects.. 
	//If it was allocated on the heap, then you have to explicitly delete it yourself
	m_vObjects.clear();
	g_pCurrentUILayer = nullptr;
	g_nUILayerIndex = -1;
}
void UILayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerMouseMove, nIndex);
	pApp->RegisterEvents(LayerMouseDown, nIndex);
	pApp->RegisterEvents(LayerMouseUp, nIndex);

	pApp->RegisterEvents(LayerKey, nIndex);
	pApp->RegisterEvents(LayerKeyDown, nIndex);
	pApp->RegisterEvents(LayerKeyUp, nIndex);

	pApp->RegisterEvents(LayerWindowResize, nIndex);

	g_nUILayerIndex = nIndex;
}

void UILayer::DeRegisterUIObject(UIObject* pObj)
{
	if (!pObj) { return; }

	//Ideally you would want to delete it from the array but that could cause a lot of deallocations
	//Instead just set it to nullptr instead so that it does not receive any events.
	//If a lot of UI elements are created and deleted then you should consider doing the former instead ?
	for (unsigned int i = 0; i < m_vObjects.size(); i++)
	{
		if (m_vObjects[i] == pObj)
		{
			m_vObjects[i] = nullptr;
		}
	}
}

void UILayer::OnStart()
{
}
void UILayer::OnUpdate(float deltaTime)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject && pObject->GetIsActive())
		{
			pObject->OnUpdate(deltaTime);
		}
	}
}
bool UILayer::OnMouseMove(int x, int y)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject && pObject->GetIsActive() && pObject->OnMouseMove(x, y)) { break; }
	}
	return false;
}

bool UILayer::OnMouseDown(int nButton)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject && pObject->GetIsActive() && pObject->OnMouseDown(nButton)) { break; }
	}
	return false;
}
bool UILayer::OnMouseUp(int nButton)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject && pObject->GetIsActive() && pObject->OnMouseUp(nButton)) { break; }
	}
	return false;
}
bool UILayer::OnKey(int key)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject && pObject->GetIsActive() && pObject->OnKey(key)) { break; }
	}
	return false;
}
bool UILayer::OnKeyDown(int key)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject && pObject->GetIsActive() && pObject->OnKeyDown(key)) { break; }
	}
	return false;
}
bool UILayer::OnKeyUp(int key)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject && pObject->GetIsActive() && pObject->OnKeyUp(key)) { break; }
	}
	return false;
}

bool UILayer::OnWindowResize(int x, int y)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject && pObject->GetIsActive() && pObject->OnWindowResize(x, y)) { break;}
	}
	return false;
}

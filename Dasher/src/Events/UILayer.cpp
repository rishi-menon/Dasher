#include "UILayer.h"
#include "Application/Application.h"

UILayer* g_pCurrentUILayer = nullptr;

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
}
void UILayer::OnStart()
{
}
bool UILayer::OnMouseMove(int x, int y)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject->GetIsActive() && pObject->OnMouseMove(x, y))
		{
			break;
		}
	}
	return false;
}

bool UILayer::OnMouseDown(int nButton)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject->GetIsActive() && pObject->OnMouseDown(nButton))
		{
			break;
		}
	}
	return false;
}
bool UILayer::OnMouseUp(int nButton)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject->GetIsActive() && pObject->OnMouseUp(nButton))
		{
			break;
		}
	}
	return false;
}
bool UILayer::OnKey(int key)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject->GetIsActive() && pObject->OnKey(key))
		{
			break;
		}
	}
	return false;
}
bool UILayer::OnKeyDown(int key)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject->GetIsActive() && pObject->OnKeyDown(key))
		{
			break;
		}
	}
	return false;
}
bool UILayer::OnKeyUp(int key)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject->GetIsActive() && pObject->OnKeyUp(key))
		{
			break;
		}
	}
	return false;
}

bool UILayer::OnWindowResize(int x, int y)
{
	for (UIObject* pObject : m_vObjects)
	{
		if (pObject->GetIsActive() && pObject->OnWindowResize(x, y))
		{
			break;
		}
	}
	return false;
}

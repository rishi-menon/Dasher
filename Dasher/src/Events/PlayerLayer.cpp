#include "PlayerLayer.h"
#include "Application/Application.h"

void PlayerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	//pApp->RegisterEvents(LayerMouseMove, nIndex);
	pApp->RegisterEvents(LayerMouseDown, nIndex);
	pApp->RegisterEvents(LayerMouseUp, nIndex);

	//pApp->RegisterEvents(LayerKey, nIndex);
	pApp->RegisterEvents(LayerKeyDown, nIndex);
	pApp->RegisterEvents(LayerKeyUp, nIndex);

	pApp->RegisterEvents(LayerWindowResize, nIndex);
	
}
void PlayerLayer::OnStart()
{
	LOG_INFO("On Start");
}

void PlayerLayer::OnUpdate(float deltaTime)
{
}

bool PlayerLayer::OnMouseMove(double x, double y)  
{ 
	LOG_INFO("Mouse Move");
	return false; 
}
bool PlayerLayer::OnMouseDown(int nButton)		  
{
	LOG_INFO("Mouse Down");
	return false; 
}

bool PlayerLayer::OnMouseUp(int nButton)			  
{
	LOG_INFO("Mouse Up");
	return false; 
}

bool PlayerLayer::OnKey(int key)		 
{ 
	LOG_INFO("Key");
	return false; 
}
bool PlayerLayer::OnKeyDown(int key)  
{
	LOG_INFO("Key down");
	return false; 
}
bool PlayerLayer::OnKeyUp(int key)    
{
	LOG_INFO("Key up");
	return false;
}

bool PlayerLayer::OnWindowResize(int x, int y)  
{
	LOG_INFO("window resize");
	return false;
}

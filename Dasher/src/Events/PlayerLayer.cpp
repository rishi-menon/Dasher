#include "PlayerLayer.h"
#include "Application/Application.h"
#include "Renderer/Renderer.h"

PlayerLayer::PlayerLayer()
{
	PlayerLayer::ResetLayer();
}
void PlayerLayer::ResetLayer()
{
	m_vPos = glm::vec2(100, 300);
	m_vVel = glm::vec2(0, 0);
	m_bIsFalling = true;
}
void PlayerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	//pApp->RegisterEvents(LayerMouseMove, nIndex);
	//pApp->RegisterEvents(LayerMouseDown, nIndex);
	//pApp->RegisterEvents(LayerMouseUp, nIndex);

	//pApp->RegisterEvents(LayerKey, nIndex);
	pApp->RegisterEvents(LayerKeyDown, nIndex);
	pApp->RegisterEvents(LayerKeyUp, nIndex);

	//pApp->RegisterEvents(LayerWindowResize, nIndex);
	
}
void PlayerLayer::OnStart()
{
	
}

void PlayerLayer::OnUpdate(float deltaTime)
{
	if (!m_bIsFalling)
	{
		const float offset = 200; 
		float multiplier = (m_vVel.y < offset) ? 5.0f : 1.0f;
		m_vVel.y += multiplier * mc_fEnginePower;
		
	}
	else
	{
		const float offset = 0;
		float multiplier = (m_vVel.y > offset) ? 1.4f : 1.0f;
		m_vVel.y += multiplier * mc_fGravity;
	}
	m_vVel.y = (float)Math::Clamp(-1200, 1200, m_vVel.y);
	m_vPos += m_vVel * deltaTime;

	//LOG_INFO("Player pos x: {0} y: {1}", m_vVel.x, m_vVel.y);

	//Draw
	RendererShapes::Rectangle(m_vPos, mc_vSize, mc_vCol, m_Vertex);
	Renderer::DrawQuadColor(m_Vertex, m_nVertexCount, RendererShapes::indexBufferQuad, 6);
	//Renderer::DrawQuad(m_vPos, mc_vSize, mc_vCol);
}

bool PlayerLayer::OnKey(int key)		 
{ 
	return false; 
}
bool PlayerLayer::OnKeyDown(int key)  
{
	if (key == mc_nJumpKey)
	{
		m_bIsFalling = false;
	}
	return false; 
}
bool PlayerLayer::OnKeyUp(int key)    
{
	if (key == mc_nJumpKey)
	{
		m_bIsFalling = true;
	}
	return false;
}

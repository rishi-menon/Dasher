#include "PlayerLayer.h"
#include "Application/Application.h"
#include "Renderer/Renderer.h"

PlayerLayer::PlayerLayer()
{
	PlayerLayer::ResetLayer();
}
void PlayerLayer::ResetLayer()
{
	m_vPos = glm::vec2(100, 800);
	m_vVel = glm::vec2(0, 0);

		//To Do: implement a health API and a take damage method to be called when a collision occurs
		//       Also Add a random number generator and start spawning blocks
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

	m_vPos.y = 400*(1 + glm::sin(3*Application::GetGameTime()));

	//Draw
	RendererShapes::Rectangle(m_Vertex, m_vPos, mc_vSize, m_vCol);
	Renderer::DrawQuadColor(m_Vertex, RendererShapes::ShapeQuad);
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
void PlayerLayer::TakeDamage(double damage)
{
	if (damage >= 5)
	{
 		m_vCol = { 1.0, 0.2, 0.2, 1.0 };
	}
	else
	{
		m_vCol = { 0.5, 0.4, 0.8,1.0 };
	}
}
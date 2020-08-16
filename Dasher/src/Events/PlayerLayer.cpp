#include "PlayerLayer.h"
#include "Application/Application.h"
#include "Renderer/Renderer.h"

PlayerLayer::PlayerLayer()
{
	PlayerLayer::ResetLayer();
}
void PlayerLayer::ResetLayer()
{
	m_dPhaseAngle = 0;
	m_dAngVelocity = 1;
	m_dApparantVelocityX = 400;

	m_nWidth = Application::GetCurrentApp()->GetWidth();
	m_nHeight = Application::GetCurrentApp()->GetHeight();
	m_vPos = glm::vec2 { 80, 400 };
	m_dAmplitude = (m_nHeight * 0.5 - m_fAmplitudeOffset);
}
void PlayerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
	pApp->RegisterEvents(LayerMouseMove, nIndex);
	//pApp->RegisterEvents(LayerMouseDown, nIndex);
	//pApp->RegisterEvents(LayerMouseUp, nIndex);

	//pApp->RegisterEvents(LayerKey, nIndex);
	//pApp->RegisterEvents(LayerKeyDown, nIndex);
	//pApp->RegisterEvents(LayerKeyUp, nIndex);

	//pApp->RegisterEvents(LayerWindowResize, nIndex);
	
}
void PlayerLayer::OnStart()
{
}

void PlayerLayer::OnUpdate(float deltaTime)
{
	//divide by 2
	m_vPos.y = (m_nHeight >> 1) + m_dAmplitude * glm::sin(m_dPhaseAngle);
	m_dPhaseAngle += m_dAngVelocity * deltaTime;

	////Draw
	RendererShapes::Rectangle(m_Vertex, m_vPos, mc_vSize, m_vCol);
	Renderer::DrawQuadColor(m_Vertex, RendererShapes::ShapeQuad);
	DrawTrajectory(2.2);
}

bool PlayerLayer::OnMouseMove(int x, int y)
{
	const int MouseLeftPadding = 200;
	const int MouseRightPadding = 200;

	double percentY = Math::GetPercent(MouseLeftPadding, m_nWidth- MouseRightPadding, x);
	percentY = Math::Clamp01(percentY);
	LOG_INFO(percentY);

	m_dAngVelocity = Math::Lerp(m_dAngVelocityMin, m_dAngVelocityMax, percentY);

	return false;
}
bool PlayerLayer::OnWindowResize(int x, int y)
{
	m_nWidth = x;
	m_nHeight = y;
	m_dAmplitude = (m_nHeight * 0.5 - m_fAmplitudeOffset);
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

void PlayerLayer::DrawTrajectory(double timeIntoFuture)
{
	int numOfPoints = 15;
	const glm::vec2 size = {20,20};
	const glm::vec4 color = { 1.0, 1.0, 1.0, 1.0 };

	double timeIncrement = timeIntoFuture / numOfPoints;
	
	double phase = m_dPhaseAngle;
	double angleIncrement = m_dAngVelocity *  timeIncrement;
	
	glm::vec2 pos = m_vPos;
	double posIncrement = m_dApparantVelocityX * timeIncrement;

	for (int i = 0; i < numOfPoints; i++)
	{
		phase += angleIncrement;
		pos.x += posIncrement;
		pos.y = (m_nHeight >> 1) + m_dAmplitude * glm::sin(phase);

		Renderer::DrawRectangle(pos, size, color);
	}
}
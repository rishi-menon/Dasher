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

	m_nWidth = Application::GetWidth();
	m_nHeight = Application::GetHeight();
	m_vPos = glm::vec2 { 80, 400 };
	m_dAmplitude = (m_nHeight * 0.5 - m_fAmplitudeOffset);

	m_dPointPosX = 80;
	m_dPointPhase = 0;
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
	m_vPos.y = static_cast<float>((m_nHeight >> 1) + m_dAmplitude * glm::sin(m_dPhaseAngle));
	m_dPhaseAngle += m_dAngVelocity * deltaTime;
	if (m_dPhaseAngle > 2*glm::pi<float>()) { m_dPhaseAngle -= 2 * glm::pi<float>(); }
	//Draw
	DrawTrajectory(2.2, 20);
	m_dPointPosX -= m_dApparantVelocityX * deltaTime;

	RendererShapes::Rectangle(m_Vertex, m_vPos, mc_vSize, m_vCol);
	Renderer::DrawQuadColor(m_Vertex, RendererShapes::ShapeQuad);
}

bool PlayerLayer::OnMouseMove(int x, int y)
{
	const int MouseLeftPadding = 200;
	const int MouseRightPadding = 200;

	double percentX = Math::GetPercent(MouseLeftPadding, (double)(m_nWidth- MouseRightPadding), x);
	percentX = Math::Clamp01(percentX);

	m_dAngVelocity = Math::Lerp(m_dAngVelocityMin, m_dAngVelocityMax, percentX);

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

 	m_vCol = { 1.0, 0.2, 0.2, 1.0 };

}
void PlayerLayer::TakeNoDamage()
{
	m_vCol = { 0.5, 0.4, 0.8,1.0 };
}

void PlayerLayer::DrawTrajectory(double timeIntoFuture, int numOfPoints)
{
	const glm::vec2 size = {20,20};
	const glm::vec4 color = { 1.0, 1.0, 1.0, 1.0 };

	double distanceIncrement = (timeIntoFuture / numOfPoints) * m_dApparantVelocityX;
	double phaseIncrement = (timeIntoFuture / numOfPoints) * m_dAngVelocity;

	glm::vec2 pos;
	pos.x = static_cast<float>(m_dPointPosX);
	double phase = m_dPointPhase;	//initial Phase of the point
	pos.y = static_cast<float>((m_nHeight >> 1) + m_dAmplitude * glm::sin(phase));

	for (int i = 0; i < numOfPoints; i++)
	{
		Renderer::DrawRectangle(pos, size, color);
		pos.x += static_cast<float>(distanceIncrement);
		phase += phaseIncrement;
		pos.y = static_cast<float>((m_nHeight >> 1) + m_dAmplitude * glm::sin(phase));
	}

	//move the point and recalculate the phase of that point
	if (m_dPointPosX < m_vPos.x)
	{
		m_dPointPosX += distanceIncrement;
		//xf - xi = vt... solve for time
		double deltaTimeStep = (m_vPos.x - m_dPointPosX) / m_dApparantVelocityX;
		//phi final - phi initial = wt.. Solve for phi initial
		m_dPointPhase = m_dPhaseAngle - m_dAngVelocity * deltaTimeStep;	//Phase of the point at location PosX;
	}
}

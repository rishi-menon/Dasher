#include "AbstractPlayerLayer.h"
#include "Application/Application.h"
#include "Renderer/Renderer.h"


AbstractPlayerLayer::AbstractPlayerLayer()
{
	m_dPhaseAngle = 0;
	m_dAngVelocity = 1;
	m_dApparantVelocityX = 400;

	m_nWidth = Application::GetWidth();
	m_nHeight = Application::GetHeight();
	m_vPos = glm::vec3{ 80, 400, 0.0 };
	m_dAmplitude = (m_nHeight * 0.5 - m_fAmplitudeOffset);

	m_vTrajectoryColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void AbstractPlayerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
	pApp->RegisterEvents(LayerMouseMove, nIndex);
}

void AbstractPlayerLayer::OnUpdate(float deltaTime)
{
	//Update position
	m_vPos.y = static_cast<float>((m_nHeight >> 1) + m_dAmplitude * glm::sin(m_dPhaseAngle));
	m_dPhaseAngle += m_dAngVelocity * deltaTime;
	if (m_dPhaseAngle > 2 * glm::pi<float>()) { m_dPhaseAngle -= 2 * glm::pi<float>(); }
}

void AbstractPlayerLayer::RenderPlayer (){
	RendererShapes::Rectangle(m_Vertex, m_vPos, m_vSize, m_vCol);
	Renderer::DrawQuadColor(m_Vertex, RendererShapes::ShapeQuad);
}

bool AbstractPlayerLayer::OnMouseMove(int x, int y)
{
	const int MouseLeftPadding = 200;
	const int MouseRightPadding = 200;

	double percentX = Math::GetPercent(MouseLeftPadding, (double)(m_nWidth - MouseRightPadding), x);
	percentX = Math::Clamp01(percentX);

	m_dAngVelocity = Math::Lerp(m_dAngVelocityMin, m_dAngVelocityMax, percentX);

	return false;
}

void AbstractPlayerLayer::RecalculateAngularVelocity()
{
	double x, y;
	Application::GetCurrentApp()->GetMousePos(x, y);
	AbstractPlayerLayer::OnMouseMove(static_cast<int>(x), static_cast<int>(y));
}

bool AbstractPlayerLayer::OnWindowResize(int x, int y)
{
	m_nWidth = x;
	m_nHeight = y;
	m_dAmplitude = (m_nHeight * 0.5 - m_fAmplitudeOffset);
	return false;
}

void AbstractPlayerLayer::DrawTrajectory(double& startingPointX, double& startingPhase, double timeIntoFuture, int numOfPoints)
{
	const glm::vec2 size = { 20,20 };

	double distanceIncrement = (timeIntoFuture / numOfPoints) * m_dApparantVelocityX;
	double phaseIncrement = (timeIntoFuture / numOfPoints) * m_dAngVelocity;

	glm::vec2 pos;
	pos.x = static_cast<float>(startingPointX);
	double phase = startingPhase;	//initial Phase of the point
	pos.y = static_cast<float>((m_nHeight >> 1) + m_dAmplitude * glm::sin(phase));

	for (int i = 0; i < numOfPoints; i++)
	{
		Renderer::DrawRectangle(pos, size, m_vTrajectoryColor);
		pos.x += static_cast<float>(distanceIncrement);
		phase += phaseIncrement;
		pos.y = static_cast<float>((m_nHeight >> 1) + m_dAmplitude * glm::sin(phase));
	}

	//move the point and recalculate the phase of that point
	if (startingPointX < m_vPos.x)
	{
		startingPointX += distanceIncrement;
		//xf - xi = vt... solve for time
		double deltaTimeStep = (m_vPos.x - startingPointX) / m_dApparantVelocityX;
		//phi final - phi initial = wt.. Solve for phi initial
		startingPhase = m_dPhaseAngle - m_dAngVelocity * deltaTimeStep;	//Phase of the point at location PosX;
	}
}

#include "ZenPlayerLayer.h"
#include "Application/Application.h"
#include "Renderer/Renderer.h"


ZenPlayerLayer::ZenPlayerLayer()
{
	m_dPointPosX = 80;
	m_dPointPhase = 0;
	m_vTrajectoryColor = { 1.0f, 1.0f, 1.0f, 1.0f };

}
void ZenPlayerLayer::OnStart()
{
	//Back button
	{
		const glm::vec2 sizeDefault = { 105,105 };
		const glm::vec4 colDefault = { 1.0f, 1.0f, 1.0f, 1.0f };

		const glm::vec2 sizeClick = { 105, 105 };
		const glm::vec4 colClick = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2 buttonPos = { Application::GetWidth() - 120, 110 };

		ButtonProps propDefault;
		propDefault.SetBasicProps(UITypes::ButtonBack, sizeDefault, colDefault);
		propDefault.SetTextProps("", { 40,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propClick;
		propClick.SetBasicProps(UITypes::ButtonBack_S, sizeClick, colClick);
		propClick.SetTextProps("", { 40,-20 }, 0.5, { 1,1,1,1 });


		m_BackButton.SetStateProperties(propDefault, Button::StateDefault);
		m_BackButton.SetStateProperties(propClick, Button::StateSelected);
		m_BackButton.SetStateProperties(propClick, Button::StateClicked);

		m_BackButton.SetPosition(buttonPos);
		m_BackButton.SetButttonClickEvent([](void* userData) {
			Application::GetCurrentApp()->SetNextMenu(Menu::MainMenu, userData);
			});

		constexpr int nEscapeKey = 256;
		m_BackButton.SetOptionalKey(nEscapeKey);
	}
}
void ZenPlayerLayer::OnUpdate(float deltaTime)
{
	AbstractPlayerLayer::OnUpdate(deltaTime);
	
	//Draw
	if (g_bShowTrajectoryEnabled)
	{
		AbstractPlayerLayer::DrawTrajectory(m_dPointPosX, m_dPointPhase, 2.2, 20);
		m_dPointPosX -= m_dApparantVelocityX * deltaTime;
	}

	RendererShapes::Rectangle(m_Vertex, m_vPos, m_vSize, m_vCol);
	Renderer::DrawQuadColor(m_Vertex, RendererShapes::ShapeQuad);
}

void ZenPlayerLayer::TakeDamage(double damage)
{
 	m_vCol = { 1.0, 0.2, 0.2, 1.0 };
}
void ZenPlayerLayer::TakeNoDamage()
{
	m_vCol = { 0.5, 0.4, 0.8,1.0 };
}

#include "ZenPlayerLayer.h"
#include "Application/Application.h"
#include "Renderer/Renderer.h"
#include "BlockSpawnerFunc/DefaultSpawnerFunc.h"


ZenPlayerLayer::ZenPlayerLayer() :
	m_nCurrentSpeed (0),
	m_bCanTakeDamage(true),
	m_bIsColliding(false),
	m_dNextDamageTime (0.0)
{
	m_dPointPosX = 80;
	m_dPointPhase = 0;
	m_vTrajectoryColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	g_fBlockSpeed = 500;
	m_dApparantVelocityX = 500;

	/////////////

	m_vAngularVelocities[0] = { 1, 6 };
	m_vSizes[0] = { 70, 70 };

	//m_vAngularVelocities[1] = { 3, 8 };
	//ßm_vSizes[1] = { 45, 45 };

	AbstractPlayerLayer::m_dAngVelocityMin = m_vAngularVelocities[m_nCurrentSpeed].x;
	AbstractPlayerLayer::m_dAngVelocityMax = m_vAngularVelocities[m_nCurrentSpeed].y;
	AbstractPlayerLayer::m_vSize = m_vSizes[m_nCurrentSpeed];

}
void ZenPlayerLayer::OnStart()
{
	//Back button
	{
		const glm::vec2 sizeDefault = { 105,105 };
		const glm::vec4 colDefault = { 1.0f, 1.0f, 1.0f, 1.0f };

		const glm::vec2 sizeClick = { 105, 105 };
		const glm::vec4 colClick = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec3 buttonPos = { Application::GetWidth() - 120, 110, 0.8f };

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

	ZenPlayerLayer::OnWindowResize(Application::GetWidth(), Application::GetHeight());
}
void ZenPlayerLayer::OnUpdate(float deltaTime)
{
	AbstractPlayerLayer::OnUpdate(deltaTime);
	
	if (!m_bIsColliding && !m_bCanTakeDamage && Application::GetGameTime() > m_dNextDamageTime)
	{
		m_bCanTakeDamage = true;
	}

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
	if (m_bCanTakeDamage)
	{
		m_bCanTakeDamage = false;
		m_bIsColliding = true;
		//switch speeds
		m_nCurrentSpeed++;
		m_nCurrentSpeed %= Speeds;

		AbstractPlayerLayer::m_dAngVelocityMin = m_vAngularVelocities[m_nCurrentSpeed].x;
		AbstractPlayerLayer::m_dAngVelocityMax = m_vAngularVelocities[m_nCurrentSpeed].y;
		AbstractPlayerLayer::m_vSize = m_vSizes[m_nCurrentSpeed];
		AbstractPlayerLayer::RecalculateAngularVelocity();
	}
}
void ZenPlayerLayer::TakeNoDamage()
{
	m_vCol = { 0.5, 0.4, 0.8,1.0 };
	m_bCanTakeDamage = false;
	m_bIsColliding = false;
	m_dNextDamageTime = Application::GetGameTime() + 1.0f;
}

bool ZenPlayerLayer::OnWindowResize(int x, int y)
{
	AbstractPlayerLayer::OnWindowResize(x, y);
	float posX = x - 120.0f;
	if (posX < 20) { posX = 20; }

	const glm::vec3 buttonPos = { posX, 110, 0.0f };
	m_BackButton.SetPosition(buttonPos);
	return false;
}
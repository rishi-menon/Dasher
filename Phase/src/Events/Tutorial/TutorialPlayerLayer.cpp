#include "TutorialPlayerLayer.h"
#include "Renderer/Renderer.h"
#include "Application/Application.h"
#include "TutorialBlockSpawnerLayer.h"

TutorialPlayerLayer::TutorialPlayerLayer() :
	m_bLayerIsPaused(true),
	m_pTutorialSpawner (nullptr)
{
	m_dPointPosX = 80;
	m_dPointPhase = 0;
	m_dApparantVelocityX = 400;
}
void TutorialPlayerLayer::OnStart()
{
	AbstractPlayerLayer::OnStart();
	const std::vector<Layer*>& layers = Application::GetCurrentApp()->GetLayers();
	for (Layer* pLayer : layers)
	{
		TutorialBlockSpawnerLayer* spawner = dynamic_cast<TutorialBlockSpawnerLayer*>(pLayer);
		if (spawner)
		{
			m_pTutorialSpawner = spawner;
			break;
		}
	}

	ASSERT(m_pTutorialSpawner, "The tutorial spawner does not exist");

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
}

void TutorialPlayerLayer::OnUpdate(float deltaTime)
{
	if (!m_bLayerIsPaused)
	{
		//Draw Trajectory
		m_dApparantVelocityX = 400;	//The player layer changes this variable to increase the speed
		AbstractPlayerLayer::DrawTrajectory(m_dPointPosX, m_dPointPhase, 2.2, 20);
		m_dPointPosX -= m_dApparantVelocityX * deltaTime;
		AbstractPlayerLayer::OnUpdate(deltaTime);
	}

	//Render player
	RendererShapes::Rectangle(m_Vertex, m_vPos, m_vSize, m_vCol);
	Renderer::DrawQuadColor(m_Vertex, RendererShapes::ShapeQuad);
}

void TutorialPlayerLayer::ResetPosition()
{
	m_dPointPosX = 80;
	m_dPointPhase = 0;

	m_vPos = glm::vec3{ 80, 400, 0.0 };
	m_dApparantVelocityX = 400;
}

void TutorialPlayerLayer::TakeDamage(double damage)
{
	TutorialStage stage = m_pTutorialSpawner->GetCurrentStage();
	m_pTutorialSpawner->RestartCurrentStage();
}
void TutorialPlayerLayer::TakeNoDamage()
{
}

bool TutorialPlayerLayer::OnWindowResize(int x, int y)
{
	AbstractPlayerLayer::OnWindowResize(x, y);
	{
		float posX = x - 120.0f;
		if (posX < 20) { posX = 20; }

		const glm::vec3 buttonPos = { posX, 110, 0.8f };
		m_BackButton.SetPosition(buttonPos);
	}
	return false;
}
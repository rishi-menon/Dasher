#include "TutorialPlayerLayer.h"
#include "Renderer/Renderer.h"
#include "Application/Application.h"
#include "TutorialBlockSpawnerLayer.h"
#include "Events/Player/NormalPlayerLayer.h"

TutorialPlayerLayer::TutorialPlayerLayer() :
	m_bLayerIsPaused(true),
	m_bStartedPhasing(false),
	m_pTutorialSpawner (nullptr),
	m_pPlayerUI (nullptr)
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
		if (!m_pTutorialSpawner)
		{
			TutorialBlockSpawnerLayer* spawner = dynamic_cast<TutorialBlockSpawnerLayer*>(pLayer);
			if (spawner)
			{
				m_pTutorialSpawner = spawner;
			}
		}
		
		if (!m_pPlayerUI)
		{
			NormalPlayerLayerUI* layerUI = dynamic_cast<NormalPlayerLayerUI*>(pLayer);
			if (layerUI)
			{
				m_pPlayerUI = layerUI;
			}
		}
	}

	ASSERT(m_pTutorialSpawner, "The tutorial spawner does not exist");
	ASSERT(m_pPlayerUI, "The normal player UI  does not exist inside tutorial player layer");

	//Back button
#if 0
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
#else
	//m_BackButton.SetIsActive(false);
#endif

	m_DefData.bIsAlive = true;
	m_DefData.dScore = 0.0;
	m_DefData.dPhaseTimeTotal = 2.0f;
	m_DefData.fPhaseTimeRemaining = m_DefData.dPhaseTimeTotal;
	m_DefData.nLivesUsed = 0;
}

void TutorialPlayerLayer::OnUpdate(float deltaTime)
{
	if (!m_bLayerIsPaused)
	{
		if (!m_bStartedPhasing)
		{
			m_DefData.fPhaseTimeRemaining += deltaTime * 0.2;
			if (m_DefData.fPhaseTimeRemaining > m_DefData.dPhaseTimeTotal)
			{
				m_DefData.fPhaseTimeRemaining = m_DefData.dPhaseTimeTotal;
			}
		}
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
void TutorialPlayerLayer::TutorialOver()
{
	if (m_pPlayerUI)
	{
		m_pPlayerUI->GetBackButton().ManualClick();
	}
}

void TutorialPlayerLayer::ResetPosition()
{
	m_dPointPosX = 80;
	m_dPointPhase = 0;

	m_vPos = glm::vec3{ 80, 400, 0.0 };
	m_dApparantVelocityX = 400;
}

void TutorialPlayerLayer::StartStage(TutorialStage stage)
{
	ResetPosition();
	m_DefData.dPhaseTimeTotal = 2.0f;
	m_DefData.fPhaseTimeRemaining = m_DefData.dPhaseTimeTotal;

	if (!m_pPlayerUI) { ASSERT(false, "Player UI layer was nullptr inside StartStage"); return; }

	NormalPlayerLayerUI::Data& data = m_pPlayerUI->GetData();

	data = NormalPlayerLayerUI::Data();	//resets everything to nullptr;

	switch (stage)
	{
		case TutorialStage::PlayFriendlySpikes:
		{
			data.pbIsAlive = &m_DefData.bIsAlive;
			data.pnLivesUsed = &m_DefData.nLivesUsed;
			break;
		}
		case TutorialStage::PlayPhasing:
		{
			data.pbIsAlive = &m_DefData.bIsAlive;
			data.pnLivesUsed = &m_DefData.nLivesUsed;
			data.pdPhaseTimeTotal= &m_DefData.dPhaseTimeTotal;
			data.pfPhaseTimeRemaining = &m_DefData.fPhaseTimeRemaining;
			break;
		}
	}

	m_pPlayerUI->SetLifeUI();
}

void TutorialPlayerLayer::TakeDamage(double damage)
{
	if (m_bStartedPhasing) { return; }

	m_pTutorialSpawner->RestartCurrentStage();
	
	m_DefData.nLivesUsed++;
	if (m_DefData.nLivesUsed >= NormalPlayerLayer::PlayerLives)
	{
		m_DefData.nLivesUsed = 0;
	}

	if (m_pPlayerUI) { m_pPlayerUI->SetLifeUI(); }
}
void TutorialPlayerLayer::TakePhaseDamage()
{
	float deltaTime = Application::GetCurrentApp()->GetDeltaTime();
	m_DefData.fPhaseTimeRemaining -= deltaTime;
	
	if (m_DefData.fPhaseTimeRemaining < 0.1)
	{
		m_DefData.fPhaseTimeRemaining = 0.1;
	}
	m_bStartedPhasing = true;
}
void TutorialPlayerLayer::TakeNoDamage()
{
	m_bStartedPhasing = false;
}

bool TutorialPlayerLayer::OnWindowResize(int x, int y)
{
	AbstractPlayerLayer::OnWindowResize(x, y);
	return false;
}
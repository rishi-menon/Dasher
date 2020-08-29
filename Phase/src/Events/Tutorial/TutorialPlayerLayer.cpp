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
	NormalPlayerLayer::OnStart();
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
}

void TutorialPlayerLayer::OnUpdate(float deltaTime)
{
	if (m_bLayerIsPaused)
	{
		//Render player
		RendererShapes::Rectangle(m_Vertex, m_vPos, m_vSize, m_vCol);
		Renderer::DrawQuadColor(m_Vertex, RendererShapes::ShapeQuad);
	}
	else
	{
		//Draw Trajectory
		m_dApparantVelocityX = 400;	//The player layer changes this variable to increase the speed
		AbstractPlayerLayer::DrawTrajectory(m_dPointPosX, m_dPointPhase, 2.2, 20);
		m_dPointPosX -= m_dApparantVelocityX * deltaTime;
		SetScore(-10);	//This prevents the score from being rendered
		NormalPlayerLayer::OnUpdate(deltaTime);
	}
}

void TutorialPlayerLayer::ResetPosition()
{
	m_dPointPosX = 80;
	m_dPointPhase = 0;

	m_vPos = glm::vec3{ 80, 400, 0.0 };
	m_dApparantVelocityX = 400;

	SetLives(0);
	SetSpeedOnDamage();	//Hack to get the speed to reset if need be
}

void TutorialPlayerLayer::TakeDamage(double damage)
{
	TutorialStage stage = m_pTutorialSpawner->GetCurrentStage();
	if (stage != TutorialStage::PlayWarningCollision)
	{
		m_pTutorialSpawner->RestartCurrentStage();
	}
	else
	{
		NormalPlayerLayer::TakeDamage(damage);
	}
}
void TutorialPlayerLayer::TakeNoDamage()
{
	TutorialStage stage = m_pTutorialSpawner->GetCurrentStage();
	if (stage == TutorialStage::PlayWarningCollision)
	{
		NormalPlayerLayer::TakeNoDamage();
	}
}
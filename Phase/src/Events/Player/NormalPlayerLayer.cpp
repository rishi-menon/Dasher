#include "NormalPlayerLayer.h"
#include "Renderer/Renderer.h"
#include "Application/Application.h"

#include "BlockSpawnerFunc/DefaultSpawnerFunc.h"
#include "AssetManagement/StandardTexture.h"
#include "Events/BlockSpawnerLayer.h"
#include "NormalPlayerLayerUI.h"


NormalPlayerLayer::NormalPlayerLayer() :
	m_bCanCollide (true),
	m_bIsColliding(false),
	m_bIsAlive (true),
	m_bStartedPhasing(false),

	m_nLivesUsed (0),

	m_dNextCollideTime (0.0),
	m_dImmunityTime (0.8),
	m_dScore (0.0),
	m_pFadeoutLayer (nullptr),
	m_pBlockSpawnerLayer (nullptr),
	m_pPlayerLayerUI (nullptr),

	m_dTotalPhaseTime (2.0f),
	m_dTimeTakenToRegen (8.0f),
	m_dNextPhaseRegenTime (0.0)

{
	m_vAngularVelocities[0] = { 1, 6 };
	m_vSizes[0] = { 70, 70 };

	m_vAngularVelocities[1] = { 1.5, 6.5 };
	m_vSizes[1] = { 60, 60 };

	m_vAngularVelocities[2] = { 2, 7 };
	m_vSizes[2] = { 55, 55 };

	m_vAngularVelocities[3] = { 2.5, 7.5 };
	m_vSizes[3] = { 45, 45 };

	m_dPhaseTimeRemaining = m_dTotalPhaseTime;

	AbstractPlayerLayer::m_dAngVelocityMin = m_vAngularVelocities[m_nLivesUsed].x;
	AbstractPlayerLayer::m_dAngVelocityMax = m_vAngularVelocities[m_nLivesUsed].y;
	AbstractPlayerLayer::m_vSize = m_vSizes[m_nLivesUsed];
}

void NormalPlayerLayer::OnStart()
{
	AbstractPlayerLayer::OnStart();

	//Get the fade out layer to play an animation when the player dies
	const std::vector<Layer*>& layers = Application::GetCurrentApp()->GetLayers();
	for (Layer* pLayer : layers)
	{
		if (!m_pFadeoutLayer)
		{
			FadeoutScreenLayer* pFadeLayer = dynamic_cast <FadeoutScreenLayer*> (pLayer);
			if (pFadeLayer)
			{
				m_pFadeoutLayer = pFadeLayer;
			}
		}
		if (!m_pBlockSpawnerLayer)
		{
			BlockSpawnerLayer* pBlockLayer = dynamic_cast <BlockSpawnerLayer*> (pLayer);
			if (pBlockLayer)
			{
				m_pBlockSpawnerLayer = pBlockLayer;
			}
		}
		if (!m_pPlayerLayerUI)
		{
			NormalPlayerLayerUI* pPlayerLayerUI = dynamic_cast<NormalPlayerLayerUI*> (pLayer);
			if (pPlayerLayerUI)
			{
				m_pPlayerLayerUI = pPlayerLayerUI;
			}
		}
	}

	if (m_pFadeoutLayer)
	{
		m_pFadeoutLayer->SetAlphaRange({ 0.0f, 0.7f });
		m_pFadeoutLayer->SetAnimationTime(0.9f);
		m_pFadeoutLayer->SetColor({0.0f, 0.0f, 0.0f});
	}
	else
	{
		LOG_WARN("Fade out layer was not found");
	}


	if (!m_pBlockSpawnerLayer)
	{
		LOG_WARN("Block spawner layer was not found in normal player layer");
	}

	if (!m_pPlayerLayerUI)
	{
		LOG_WARN("normal player layer UI was not found in normal player layer");

	}
	
	NormalPlayerLayer::OnWindowResize(Application::GetWidth(), Application::GetHeight());
}

//////////////////////////////////////////////////////////////////////////////
//																			//
//////////////////////////////////////////////////////////////////////////////

void NormalPlayerLayer::OnUpdate(float deltaTime)
{
	if (m_bIsAlive)
	{
		AbstractPlayerLayer::OnUpdate(deltaTime);

		double dCurrentTime = Application::GetGameTime();
		if (!m_bCanCollide && !m_bIsColliding && dCurrentTime > m_dNextCollideTime)
		{
			m_bCanCollide = true;
			m_vCol = { 0.5, 0.4, 0.8,1.0 };
		}

		//Phase time
		if (m_dPhaseTimeRemaining < m_dTotalPhaseTime && m_dNextPhaseRegenTime > 0 && dCurrentTime > m_dNextPhaseRegenTime)
		{
			bool bCantPhase = (m_dPhaseTimeRemaining < 0.0);
			m_dPhaseTimeRemaining += deltaTime / m_dTimeTakenToRegen;
			if (m_dPhaseTimeRemaining > m_dTotalPhaseTime)
			{
				m_dPhaseTimeRemaining = m_dTotalPhaseTime;
			}

			if (bCantPhase && m_dPhaseTimeRemaining > 0.0)
			{
				//it just went from -ve to +ve... Recalculate the phase
				double x, y;
				Application::GetCurrentApp()->GetMousePos(x, y);
				m_pBlockSpawnerLayer->RecalculateBlockPhase((float)x);
			}
		}

		//Score
		m_dScore += deltaTime;
		
		//Render player
		RendererShapes::Rectangle(m_Vertex, m_vPos, m_vSize, m_vCol);
		Renderer::DrawQuadColor(m_Vertex, RendererShapes::ShapeQuad);

		constexpr float blockSpeedMultiplier = 1.0f / 0.75f;	//denominator is the Time taken to increase the block speed by 1 unit
		g_fBlockSpeed += deltaTime * blockSpeedMultiplier;
		m_dApparantVelocityX = g_fBlockSpeed;
	}
	else
	{
		
	}
}

void NormalPlayerLayer::TakeDamage(double damage)
{
	if (m_bCanCollide && !m_bStartedPhasing)
	{
		m_bCanCollide = false;
		m_bIsColliding = true;
		m_vCol = { 1.0, 81.0/255, 81.0/255, 0.7 };

		//Take damage
		m_nLivesUsed++;

		if (m_pPlayerLayerUI)
		{
			m_pPlayerLayerUI->SetLifeUI();
		}
		if (m_nLivesUsed < PlayerLives)
		{
			AbstractPlayerLayer::m_dAngVelocityMin = m_vAngularVelocities[m_nLivesUsed].x;
			AbstractPlayerLayer::m_dAngVelocityMax = m_vAngularVelocities[m_nLivesUsed].y;
			AbstractPlayerLayer::m_vSize = m_vSizes[m_nLivesUsed];
			AbstractPlayerLayer::RecalculateAngularVelocity();
		}
		else
		{
			NormalPlayerLayer::Die(); // no more lives
		}
	}
}

void NormalPlayerLayer::TakeNoDamage()
{
	if (m_bIsColliding)
	{
		m_bIsColliding = false;
		m_dNextCollideTime = Application::GetGameTime() + m_dImmunityTime;
	}
	
	if (m_dNextPhaseRegenTime < 0)
	{
		//Player just finished a phase (time mightve ran out so we cant use m_bIsCollididng as he mightve phased and then midway collided)
		m_bStartedPhasing = false;
		constexpr double dWaitTimeBeforeRegen = 5;
		m_dNextPhaseRegenTime = Application::GetGameTime() + dWaitTimeBeforeRegen;
	}
}

void NormalPlayerLayer::TakePhaseDamage()
{
	m_bStartedPhasing = true;

	if (!m_bCanCollide) { return; }

	float deltaTime = Application::GetCurrentApp()->GetDeltaTime();

	m_dNextPhaseRegenTime = -1;	//prevents regen

	//small negative value instead of exactly 0.0 so that the CanPhase check ( x > 0) consistantly works

	bool bCanPhase = (m_dPhaseTimeRemaining > 0);
	m_dPhaseTimeRemaining -= deltaTime;
	if (m_dPhaseTimeRemaining <= -0.05)
	{
		m_dPhaseTimeRemaining = -0.05;
	}

	if (m_pBlockSpawnerLayer && bCanPhase && m_dPhaseTimeRemaining <= 0.0)
	{
		//it just went from +ve to -ve... Recalculate the phase
		double x, y;
		Application::GetCurrentApp()->GetMousePos(x, y);
		m_pBlockSpawnerLayer->RecalculateBlockPhase((float)x);
	}
}

void NormalPlayerLayer::Die()
{
	m_bIsAlive = false;
	if (m_pFadeoutLayer)
	{
		m_pFadeoutLayer->BeginFading();
	}
	
	if (m_pPlayerLayerUI)
	{
		m_pPlayerLayerUI->PlayerHasDied();
	}
	//Invalidate position
	m_vPos = { -1000.0f, -1000.0f, 0.0f };
	RendererShapes::Rectangle(m_Vertex, m_vPos, m_vSize, m_vCol);
	
	if (m_pPlayerLayerUI)
	{
		m_pPlayerLayerUI->SetLifeUI();
	}
}

void NormalPlayerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	AbstractPlayerLayer::RegisterEvents(pApp, nIndex);
}

bool NormalPlayerLayer::OnWindowResize(int x, int y)
{
	AbstractPlayerLayer::OnWindowResize(x, y);

	return false;
}
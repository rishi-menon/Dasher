#pragma once

#include "AbstractPlayerLayer.h"
#include "UI/Button.h"
#include "Events/FadeoutScreenLayer.h"

class BlockSpawnerLayer;
class NormalPlayerLayerUI;

class NormalPlayerLayer : public AbstractPlayerLayer
{
	friend class NormalPlayerLayerUI;
public:
	enum : int { PlayerLives = 4 };
	NormalPlayerLayer();
	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;
	
	virtual void TakeDamage(double damage) override;
	virtual void TakePhaseDamage() override;
	virtual void TakeNoDamage() override;

	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual bool OnWindowResize(int x, int y) override;

	virtual bool CanPhase() const override { return m_dPhaseTimeRemaining > 0.0; }

private:
	void Die();

private:
	glm::vec2 m_vAngularVelocities[PlayerLives];
	glm::vec2 m_vSizes[PlayerLives];

private:
	bool m_bCanCollide;
	bool m_bIsColliding;
	bool m_bIsAlive;
	bool m_bStartedPhasing;

	int m_nLivesUsed;

	double m_dNextCollideTime;
	const double m_dImmunityTime;

	double m_dScore;

	const double m_dTotalPhaseTime;
	const double m_dTimeTakenToRegen;
	double m_dPhaseTimeRemaining;

	double m_dNextPhaseRegenTime;

	FadeoutScreenLayer* m_pFadeoutLayer;
	BlockSpawnerLayer* m_pBlockSpawnerLayer;
	NormalPlayerLayerUI* m_pPlayerLayerUI;
};
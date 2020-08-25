#pragma once

#include "AbstractPlayerLayer.h"
#include "UI/Button.h"
#include "Events/FadeoutScreenLayer.h"

class NormalPlayerLayer : public AbstractPlayerLayer
{
public:
	NormalPlayerLayer();
	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;
	
	virtual void TakeDamage(double damage) override;
	virtual void TakeNoDamage() override;

	virtual void RegisterEvents(Application* pApp, int nIndex) override;

protected:
	inline void SetScore(double score) { m_dScore = score; }	//This is mainly for the tutorial layer
	inline Button& GetBackButton() { return m_BackButton; }
	inline void SetLives(int livesUsed) { m_nLivesUsed = livesUsed; }
	void SetSpeedOnDamage();

private:
	void Die();

private:
	enum : int {PlayerLives = 2};

	const glm::vec2 m_vScorePos;
	const float m_fScoreScale;
	const glm::vec4 m_vScoreCol;

	glm::vec2 m_vAngularVelocities[PlayerLives];
	glm::vec2 m_vSizes[PlayerLives];

private:
	Button m_BackButton;
	Button m_restartButton;

	bool m_bCanCollide;
	bool m_bIsColliding;
	bool m_bIsAlive;

	int m_nLivesUsed;

	double m_dNextCollideTime;
	const double m_dImmunityTime;

	double m_dScore;

	FadeoutScreenLayer* m_pFadeoutLayer;
};
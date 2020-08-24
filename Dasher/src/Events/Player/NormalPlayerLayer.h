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

#if 1
	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual bool OnKeyDown(int key) override;
#endif

private:
	void Die();

private:
	const glm::vec2 m_vScorePos;
	const float m_fScoreScale;
	const glm::vec4 m_vScoreCol;

private:
	Button m_BackButton;
	Button m_restartButton;

	bool m_bCanCollide;
	bool m_bIsColliding;
	bool m_bIsAlive;

	double m_dNextCollideTime;
	const double m_dImmunityTime;

	double m_dScore;

	FadeoutScreenLayer* m_pFadeoutLayer;
};
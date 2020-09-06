#pragma once

#include "Events/Layer.h"
#include "UI/UIImage.h"
#include "UI/Button.h"
#include "NormalPlayerLayer.h"
#include "Renderer/RendererVertex.h"

class NormalPlayerLayerUI : public Layer
{
public:
#if 0
	struct Data
	{
		bool* pbIsAlive;
		double* pdScore;
		int* pnLivesUsed;
		float* pfPhaseTimeRemaining;
		double* pdPhaseTimeTotal;
	};
#endif
public:
	NormalPlayerLayerUI();

	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	
	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;	//in seconds
	virtual bool OnWindowResize(int x, int y) override;

	void SetLifeUI();
	void PlayerHasDied();	//Change the score to the center

private:
	void DrawPhaseTimeRemainingBar();
private:
	const glm::vec2 m_vHealthUISize;
	const float m_fHealthUIOffsetX;

	glm::vec2 m_vScorePos;

	glm::vec2 m_vPhaseTimeBaseSize;
	glm::vec3 m_vPhaseTimeBasePos;
private:
	NormalPlayerLayer* m_pNormalPlayerLayer;

	RendererVertex m_vertexPhaseTimeBase[4];	//Base layer

	Button m_BackButton;
	Button m_restartButton;
	UIImage m_HealthUI[NormalPlayerLayer::PlayerLives];
};
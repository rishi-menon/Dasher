#pragma once

#include "Events/Layer.h"
#include "UI/UIImage.h"
#include "UI/Button.h"
#include "Renderer/RendererVertex.h"
#include "Log.h"
#include "NormalPlayerLayer.h"

class NormalPlayerLayer;
class NormalPlayerLayerUI : public Layer
{
public:
	struct Data
	{
		Data();
		void SetDataNormalPlayerLayer(NormalPlayerLayer* playerLayer);
#if 0
		inline bool& GetIsAlive() {
#ifdef RM_DEBUG
			if (!pbIsAlive)
			{
				ASSERT(false, "Data: Is Alive is nullptr");
			}
#endif
			return *pbIsAlive; 
		}

		inline double& GetScore() {
#ifdef RM_DEBUG
			if (!pdScore)
			{
				ASSERT(false, "Data: Is Alive is nullptr");
			}
#endif
			return *pdScore;
		}

		inline int& GetLivesUsed() {
#ifdef RM_DEBUG
			if (!pnLivesUsed)
			{
				ASSERT(false, "Data: Is Alive is nullptr");
			}
#endif
			return *pnLivesUsed;
		}

		inline double& GetPhaseTimeRemaining() {
#ifdef RM_DEBUG
			if (!pfPhaseTimeRemaining)
			{
				ASSERT(false, "Data: Is Alive is nullptr");
			}
#endif
			return *pfPhaseTimeRemaining;
		}

		inline double GetPhaseTimeTotal() {
#ifdef RM_DEBUG
			if (!pdPhaseTimeTotal)
			{
				ASSERT(false, "Data: Is Alive is nullptr");
			}
#endif
			return *pdPhaseTimeTotal;
		}
#endif
		bool* pbIsAlive;
		double* pdScore;
		int* pnLivesUsed;
		double* pfPhaseTimeRemaining;
		const double* pdPhaseTimeTotal;
	};

	struct DefaultData
	{
		DefaultData() = default;
		DefaultData(const DefaultData&) = default;
		bool bIsAlive;
		double dScore;
		int nLivesUsed;
		double fPhaseTimeRemaining;
		double dPhaseTimeTotal;
	};

public:
	NormalPlayerLayerUI();

	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	
	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;	//in seconds
	virtual bool OnWindowResize(int x, int y) override;

	void SetLifeUI();
	void PlayerHasDied();	//Change the score to the center

	inline Data& GetData() { return m_data; }
	inline Button& GetBackButton() { return m_BackButton; }

private:
	void DrawPhaseTimeRemainingBar();
private:
	const glm::vec2 m_vHealthUISize;
	const float m_fHealthUIOffsetX;

	glm::vec2 m_vScorePos;

	glm::vec2 m_vPhaseTimeBaseSize;
	glm::vec3 m_vPhaseTimeBasePos;
private:
	Data m_data;

	RendererVertex m_vertexPhaseTimeBase[4];	//Base layer

	Button m_BackButton;
	Button m_restartButton;
	UIImage m_HealthUI[NormalPlayerLayer::PlayerLives];
};
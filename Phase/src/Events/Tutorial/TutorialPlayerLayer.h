#pragma once
#include "Events/Player/AbstractPlayerLayer.h"
#include "UI/Button.h"
#include "Events/Player/NormalPlayerLayerUI.h"
#include "Events/Tutorial/TutorialBlockSpawnerLayer.h"

class TutorialBlockSpawnerLayer;
class TutorialPlayerLayer : public AbstractPlayerLayer
{
public:
	TutorialPlayerLayer();

	//Events
	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual bool OnWindowResize(int x, int y) override;

	void SetPauseTutorial(bool pause) { m_bLayerIsPaused = pause; }
	void TutorialOver();
	
	virtual void TakeDamage(double damage) override;
	virtual void TakePhaseDamage() override;
	virtual void TakeNoDamage() override;

	//Gets called when the stage progresses
	void StartStage(TutorialStage stage);

	inline glm::vec3 GetPlayerPos() const { return m_vPos; }

private:
	inline void ResetPosition();

private:
	bool m_bLayerIsPaused;
	bool m_bStartedPhasing;
	TutorialBlockSpawnerLayer* m_pTutorialSpawner;
	NormalPlayerLayerUI* m_pPlayerUI;

	double m_dPointPosX;	//This is the x coordinate of the first point of the trajectory
	double m_dPointPhase;	//This is the phase of the first point

	NormalPlayerLayerUI::DefaultData m_DefData;
};
#pragma once
#include "Events/Player/AbstractPlayerLayer.h"
#include "UI/Button.h"

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
	inline void TutorialOver() { m_BackButton.ManualClick(); }
	void ResetPosition();

	virtual void TakeDamage(double damage) override;
	virtual void TakeNoDamage() override;
private:
	bool m_bLayerIsPaused;
	TutorialBlockSpawnerLayer* m_pTutorialSpawner;
	Button m_BackButton;

	double m_dPointPosX;	//This is the x coordinate of the first point of the trajectory
	double m_dPointPhase;	//This is the phase of the first point
};
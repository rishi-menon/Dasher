#pragma once
#include "Events/Player/NormalPlayerLayer.h"

class TutorialBlockSpawnerLayer;
class TutorialPlayerLayer : public NormalPlayerLayer
{
public:
	TutorialPlayerLayer();
	virtual void OnStart() override;

	virtual void OnUpdate(float deltaTime) override;

	void SetPauseTutorial(bool pause) { m_bLayerIsPaused = pause; }

	inline void TutorialOver() { GetBackButton().ManualClick(); }
	void ResetPosition();

	virtual void TakeDamage(double damage) override;
	virtual void TakeNoDamage() override;
private:
	bool m_bLayerIsPaused;
	TutorialBlockSpawnerLayer* m_pTutorialSpawner;

	double m_dPointPosX;	//This is the x coordinate of the first point of the trajectory
	double m_dPointPhase;	//This is the phase of the first point
};
#pragma once

#include "Events/Player/AbstractPlayerLayer.h"

#include "Maths.h"
#include "Renderer/RendererVertex.h"
#include "UI/Button.h"


class ZenPlayerLayer : public AbstractPlayerLayer
{
public:
	ZenPlayerLayer();
	
	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;	//in seconds

	virtual void TakeDamage(double damage) override;
	virtual void TakeNoDamage() override;
	
	virtual bool OnWindowResize(int x, int y) override;
	
private:
	enum : int { Speeds = 1 };
	glm::vec2 m_vAngularVelocities[Speeds];
	glm::vec2 m_vSizes[Speeds];

private:
	double m_dPointPosX;	//This is the x coordinate of the first point of the trajectory
	double m_dPointPhase;	//This is the phase of the first point

	Button m_BackButton;

	int m_nCurrentSpeed;
	bool m_bCanTakeDamage;
	bool m_bIsColliding;

	double m_dNextDamageTime;
};   
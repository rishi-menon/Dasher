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

private:
	double m_dPointPosX;	//This is the x coordinate of the first point of the trajectory
	double m_dPointPhase;	//This is the phase of the first point

	Button m_BackButton;
};   
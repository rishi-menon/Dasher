#pragma once

#include "Layer.h"
#include "Maths.h"

class PlayerLayer : public Layer
{
public:
	PlayerLayer();
	
	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual void ResetLayer() override;

	virtual void OnStart();
	virtual void OnUpdate(float deltaTime);	//in seconds

	virtual bool OnKey(int key);
	virtual bool OnKeyDown(int key);
	virtual bool OnKeyUp(int key);


private:
	const int mc_nJumpKey = 32;
	
	const float mc_fEnginePower = 14;
	const float mc_fGravity = /*-27;*/ 0;

	const glm::vec2 mc_vSize = { 70,70 };
	const glm::vec4 mc_vCol =  { 0.5, 0.4, 0.8,1.0 };
private:	
	glm::vec2 m_vPos;

	glm::vec2 m_vVel;
	
	bool m_bIsFalling;
};   
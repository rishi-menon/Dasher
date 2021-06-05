#pragma once
#include "Events/Layer.h"
#include "UI/Button.h"
#include "UI/PushButton.h"
#include <string>

class MainMenuLayer : public Layer
{
public:
	MainMenuLayer();

	virtual void RegisterEvents(Application* pApp, int nIndex) override;

	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;
	
	virtual bool OnWindowResize(int x, int y) override;

private:
	const glm::vec2 m_ShowTrajTextPos;
	const float m_ShowTrajTextScale;
	const std::string m_strShowTraj;
	const glm::vec4 m_vShowTrajTextCol;
	const glm::vec2 m_ShowTrajPushButtonOffset;

private:
	Button m_buttonMainMenu;
	Button m_buttonPracticeMode;
	Button m_buttonTutorial;
	Button m_buttonCredits;
	PushButton m_pbShowTrajectory;

};
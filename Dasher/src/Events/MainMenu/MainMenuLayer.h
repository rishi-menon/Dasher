#pragma once
#include "Events/Layer.h"
#include "UI/Button.h"

class MainMenuLayer : public Layer
{
public:
	MainMenuLayer() = default;

	virtual void RegisterEvents(Application* pApp, int nIndex) override;

	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;
	
	virtual bool OnWindowResize(int x, int y) override;

private:
	Button m_buttonMainMenu;
	Button m_buttonPracticeMode;
	Button m_buttonTutorial;
	Button m_buttonCredits;
};
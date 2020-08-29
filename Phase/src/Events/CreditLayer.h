#pragma once

#include "Layer.h"
#include "UI/Button.h"
#include <string>

class CreditLayer : public Layer
{
public:
	CreditLayer();

	//Events
	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual bool OnWindowResize(int x, int y) override;
	
	virtual bool OnKeyUp(int key) override;

private:
	Button m_BackButton;

	enum { TextLineCount = 2};
	std::string m_strCredit[TextLineCount];

	glm::vec2 m_vPosText;
};
#include "CreditLayer.h"
#include "Application/Application.h"
#include "Renderer/Renderer.h"

CreditLayer::CreditLayer()
{

}
void CreditLayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
	pApp->RegisterEvents(LayerKeyUp, nIndex);
}
void CreditLayer::OnStart()
{
	m_strCredit[0] = "Made by:";
	m_strCredit[1] = "Rishi Menon";

	//Back button
	{
		const glm::vec2 sizeDefault = {105,105 };
		const glm::vec4 colDefault = { 1.0f, 1.0f, 1.0f, 1.0f };

		const glm::vec2 sizeClick = { 105, 105 };
		const glm::vec4 colClick = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec3 buttonPos = { Application::GetWidth() - 120, 110, 0.0f };

		ButtonProps propDefault;
		propDefault.SetBasicProps(UITypes::ButtonBack, sizeDefault, colDefault);
		propDefault.SetTextProps("", { 40,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propClick;
		propClick.SetBasicProps(UITypes::ButtonBack_S, sizeClick, colClick);
		propClick.SetTextProps("", { 40,-20 }, 0.5, { 1,1,1,1 });

		m_BackButton.SetStateProperties(propDefault, Button::StateDefault);
		m_BackButton.SetStateProperties(propClick, Button::StateSelected);
		m_BackButton.SetStateProperties(propClick, Button::StateClicked);

		m_BackButton.SetPosition(buttonPos);
		m_BackButton.SetButttonClickEvent([](void* userData) {
			Application::GetCurrentApp()->SetNextMenu(Menu::MainMenu, userData);
			});
	}

}

void CreditLayer::OnUpdate(float deltaTime)
{
	glm::vec2 posText = {550, 700};
	const float textScale[TextLineCount] = {0.8f, 0.6f};

	for (int i = 0; i < TextLineCount; i++)
	{
		Renderer::DrawTextColor(m_strCredit[i].c_str(), m_strCredit[i].size(), posText, textScale[i], { 0.0f, 0.0f, 0.0f, 1.0f });
		posText.y -= 120;
	}
}

bool CreditLayer::OnWindowResize(int x, int y)
{
	return false;
}

bool CreditLayer::OnKeyUp(int key)
{
	constexpr int nEscapeKey = 256;
	if (key == nEscapeKey)
	{
		m_BackButton.ManualClick();
	}
	return false;
}
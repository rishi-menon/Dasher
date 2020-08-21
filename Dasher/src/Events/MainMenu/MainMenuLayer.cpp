#include "MainMenuLayer.h"
#include "Application/Application.h"

void MainMenuLayer::RegisterEvents(Application * pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
}
	 
void MainMenuLayer::OnStart() 
{
	ButtonProps propDefault;
	propDefault.SetBasicProps (UITypes::Button0, {350, 150}, {218.0f / 256, 156.0f / 256, 0.0f, 0.9f});
	propDefault.SetTextProps("Play", { 120,-20 }, 0.5, { 1,1,1,1 });

	ButtonProps propHighlight;
	propHighlight.SetBasicProps( UITypes::Button0, {375, 160}, {1.0f, 209 / 255.0f, 91 / 255.0f, 0.75f});
	propHighlight.SetTextProps("Play", { 120,-20 }, 0.5, { 1,1,1,1 });

	ButtonProps propClick;
	propClick.SetBasicProps( UITypes::Button0, {375, 160}, {1.0f, 223 / 255.0f, 141 / 255.0f, 0.94f});
	propClick.SetTextProps("Play", { 120,-20 }, 0.5, { 1,1,1,1 });
	
	m_buttonMainMenu.SetStateProperties(propDefault, Button::StateDefault);
	m_buttonMainMenu.SetStateProperties(propClick, Button::StateClicked);
	m_buttonMainMenu.SetStateProperties(propHighlight, Button::StateSelected);
	m_buttonMainMenu.SetPosition({ 740, 800 });
	m_buttonMainMenu.SetButttonClickEvent([]() {
		Application::GetCurrentApp()->SetNextMenu(Menu::NormalMode);
	});
	
	m_buttonPracticeMode.SetIsActive(false);
	m_buttonTutorial.SetIsActive(false);
	m_buttonCredits.SetIsActive(false);
}
void MainMenuLayer::OnUpdate(float deltaTime) 
{

}

bool MainMenuLayer::OnWindowResize(int x, int y)
{
	return false;
}

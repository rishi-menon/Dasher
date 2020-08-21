#include "MainMenuLayer.h"
#include "Application/Application.h"

void MainMenuLayer::RegisterEvents(Application * pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
}
	 
void MainMenuLayer::OnStart() 
{
	ButtonProps propDefault = { UITypes::Button0, {350, 150}, {218.0f/256, 156.0f/256, 0.0f, 0.9f} };
	ButtonProps propHighlight = { UITypes::Button0, {375, 160}, {1.0f, 209/255.0f, 91/255.0f, 0.75f} };
	ButtonProps propClick = { UITypes::Button0, {375, 160}, {1.0f, 223/255.0f, 141/255.0f, 0.94f} };
	
	m_buttonMainMenu.SetStateProperties(propDefault, Button::StateDefault);
	m_buttonMainMenu.SetStateProperties(propClick, Button::StateClicked);
	m_buttonMainMenu.SetStateProperties(propHighlight, Button::StateSelected);

	//Set the state to default here so that the SetPosition will automatically regenerate the buffers
	//m_buttonMainMenu.SetCurrentState(Button::ButtonState::StateDefault);	
	m_buttonMainMenu.SetPosition({ 400, 400 });
	
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

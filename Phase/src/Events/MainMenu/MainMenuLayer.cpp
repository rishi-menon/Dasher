#include "MainMenuLayer.h"
#include "Application/Application.h"
#include "Renderer/Renderer.h"

void MainMenuLayer::RegisterEvents(Application * pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
}
	 
MainMenuLayer::MainMenuLayer() :
	//Constants
	m_ShowTrajTextPos(50, 90),
	m_ShowTrajTextScale (0.35f),
	m_strShowTraj ("Show Trajectory (Zen Mode):"),
	m_vShowTrajTextCol(1.0f, 1.0f, 1.0f, 1.0f),
	m_ShowTrajPushButtonOffset (620, 12)
{

}
void MainMenuLayer::OnStart() 
{
	//Normal Mode
	{
		const glm::vec2 sizeDefault = { 450, 150 };
		const glm::vec4 colDefault = { 218.0f / 256, 156.0f / 256, 0.0f, 0.9f };
		const glm::vec2 sizeSelected = { 470, 160 };
		const glm::vec4 colSelected = { 1.0f, 209 / 255.0f, 91 / 255.0f, 0.75f };
		const glm::vec4 colClick = { 1.0f, 223 / 255.0f, 141 / 255.0f, 0.94f };
		const glm::vec3 buttonPos = { 740, 1000, 0.0f };

		ButtonProps propDefault;
		propDefault.SetBasicProps(UITypes::Button0, sizeDefault, colDefault);
		propDefault.SetTextProps("Normal Mode", { 40,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propHighlight;
		propHighlight.SetBasicProps(UITypes::Button0, sizeSelected, colSelected);
		propHighlight.SetTextProps("Normal Mode", { 40,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propClick;
		propClick.SetBasicProps(UITypes::Button0, sizeSelected, colClick);
		propClick.SetTextProps("Normal Mode", { 40,-20 }, 0.5, { 1,1,1,1 });

		m_buttonMainMenu.SetStateProperties(propDefault, Button::StateDefault);
		m_buttonMainMenu.SetStateProperties(propClick, Button::StateClicked);
		m_buttonMainMenu.SetStateProperties(propHighlight, Button::StateSelected);
		m_buttonMainMenu.SetPosition(buttonPos);
		m_buttonMainMenu.SetButttonClickEvent([](void* userData) {
			Application::GetCurrentApp()->SetNextMenu(Menu::NormalMode, userData);
		});
	}

	//Zen Mode
	{
		const glm::vec2 sizeDefault = { 450, 150 };
		const glm::vec4 colDefault = { 218.0f / 256, 156.0f / 256, 0.0f, 0.9f };
		const glm::vec2 sizeSelected = { 470, 160 };
		const glm::vec4 colSelected = { 1.0f, 209 / 255.0f, 91 / 255.0f, 0.75f };
		const glm::vec4 colClick = { 1.0f, 223 / 255.0f, 141 / 255.0f, 0.94f };
		const glm::vec3 buttonPos = { 740, 800, 0.0f };

		ButtonProps propDefault;
		propDefault.SetBasicProps(UITypes::Button0, sizeDefault, colDefault);
		propDefault.SetTextProps("Zen Mode", { 80,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propHighlight;
		propHighlight.SetBasicProps(UITypes::Button0, sizeSelected, colSelected);
		propHighlight.SetTextProps("Zen Mode", { 80,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propClick;
		propClick.SetBasicProps(UITypes::Button0, sizeSelected, colClick);
		propClick.SetTextProps("Zen Mode", { 80,-20 }, 0.5, { 1,1,1,1 });

		m_buttonPracticeMode.SetStateProperties(propDefault, Button::StateDefault);
		m_buttonPracticeMode.SetStateProperties(propClick, Button::StateClicked);
		m_buttonPracticeMode.SetStateProperties(propHighlight, Button::StateSelected);
		m_buttonPracticeMode.SetPosition(buttonPos);
		m_buttonPracticeMode.SetButttonClickEvent([](void* userData) {
			Application::GetCurrentApp()->SetNextMenu(Menu::ZenMode, userData);
		});
	}
	//Button Tutorial
	{
		const glm::vec2 sizeDefault = { 450, 150 };
		const glm::vec4 colDefault = { 218.0f / 256, 156.0f / 256, 0.0f, 0.9f };
		const glm::vec2 sizeSelected = { 470, 160 };
		const glm::vec4 colSelected = { 1.0f, 209 / 255.0f, 91 / 255.0f, 0.75f };
		const glm::vec4 colClick = { 1.0f, 223 / 255.0f, 141 / 255.0f, 0.94f };
		const glm::vec3 buttonPos = { 740, 600, 0.0f };

		ButtonProps propDefault;
		propDefault.SetBasicProps(UITypes::Button0, sizeDefault, colDefault);
		propDefault.SetTextProps("Tutorial", { 120,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propHighlight;
		propHighlight.SetBasicProps(UITypes::Button0, sizeSelected, colSelected);
		propHighlight.SetTextProps("Tutorial", { 120,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propClick;
		propClick.SetBasicProps(UITypes::Button0, sizeSelected, colClick);
		propClick.SetTextProps("Tutorial", { 120,-20 }, 0.5, { 1,1,1,1 });

		m_buttonTutorial.SetStateProperties(propDefault, Button::StateDefault);
		m_buttonTutorial.SetStateProperties(propClick, Button::StateClicked);
		m_buttonTutorial.SetStateProperties(propHighlight, Button::StateSelected);
		m_buttonTutorial.SetPosition(buttonPos);
		m_buttonTutorial.SetButttonClickEvent([](void* userData) {
			Application::GetCurrentApp()->SetNextMenu(Menu::TutorialMode, userData);
		});
	}

	//Credits
	{
		const glm::vec2 sizeDefault = { 450, 150 };
		const glm::vec4 colDefault = { 218.0f / 256, 156.0f / 256, 0.0f, 0.9f };
		const glm::vec2 sizeSelected = { 470, 160 };
		const glm::vec4 colSelected = { 1.0f, 209 / 255.0f, 91 / 255.0f, 0.75f };
		const glm::vec4 colClick = { 1.0f, 223 / 255.0f, 141 / 255.0f, 0.94f };
		const glm::vec3 buttonPos = { 740, 400, 0.0f };

		ButtonProps propDefault;
		propDefault.SetBasicProps(UITypes::Button0, sizeDefault, colDefault);
		propDefault.SetTextProps("Credits", { 120,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propHighlight;
		propHighlight.SetBasicProps(UITypes::Button0, sizeSelected, colSelected);
		propHighlight.SetTextProps("Credits", { 120,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propClick;
		propClick.SetBasicProps(UITypes::Button0, sizeSelected, colClick);
		propClick.SetTextProps("Credits", { 120,-20 }, 0.5, { 1,1,1,1 });

		m_buttonCredits.SetStateProperties(propDefault, Button::StateDefault);
		m_buttonCredits.SetStateProperties(propClick, Button::StateClicked);
		m_buttonCredits.SetStateProperties(propHighlight, Button::StateSelected);
		m_buttonCredits.SetPosition(buttonPos);
		m_buttonCredits.SetButttonClickEvent([](void* userData) {
			Application::GetCurrentApp()->SetNextMenu(Menu::Credits, userData);
		});
	}
	
	//Show Trajectory Push Button
	{
		//Aspect ratio of the image 
		const TextureDimensions& dim = UI::GetTextureDimension(UITypes::PushButton0);
		float ar = static_cast<float>(dim.height) / static_cast<float>(dim.width);
		float width = 100;
		m_pbShowTrajectory.SetStateProperties(false, PushButtonProps(UITypes::PushButton0, { width, width * ar }, { 1,1,1,1 }));

		m_pbShowTrajectory.SetStateProperties(true, PushButtonProps(UITypes::PushButton0_E, { width, width * ar }, { 1,1,1,1 }));

		glm::vec3 pos = { m_ShowTrajTextPos.x + m_ShowTrajPushButtonOffset.x,
						  m_ShowTrajTextPos.y + m_ShowTrajPushButtonOffset.y,
						  0.0f};
		m_pbShowTrajectory.SetPos(pos);

		m_pbShowTrajectory.SetEnabled(g_bShowTrajectoryEnabled);
		m_pbShowTrajectory.SetButttonClickEvent([](bool value)
		{
			g_bShowTrajectoryEnabled = value;
		});
	}

}
void MainMenuLayer::OnUpdate(float deltaTime) 
{
	Renderer::DrawTextColor(m_strShowTraj.c_str(), m_strShowTraj.size(), m_ShowTrajTextPos, m_ShowTrajTextScale, m_vShowTrajTextCol);
}

bool MainMenuLayer::OnWindowResize(int x, int y)
{
	return false;
}

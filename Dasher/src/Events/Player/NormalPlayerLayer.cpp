#include "NormalPlayerLayer.h"
#include "Renderer/Renderer.h"
#include "Application/Application.h"
#include <string>

NormalPlayerLayer::NormalPlayerLayer() :
	// constants
	m_vScorePos (Application::GetWidth() - 280.0f, Application::GetHeight() - 140.0f),
	m_fScoreScale (0.7f),
	m_vScoreCol (1.0f, 1.0f, 1.0f, 1.0f),

	m_bCanCollide (true),
	m_bIsColliding(false),
	m_bIsAlive (true),

	m_dNextCollideTime (0.0),
	m_dImmunityTime (2.0),
	m_dScore (0.0),
	m_pFadeoutLayer (nullptr)
{
}

void NormalPlayerLayer::OnStart()
{
	//Back button
	{
		const glm::vec2 sizeDefault = { 105,105 };
		const glm::vec4 colDefault = { 1.0f, 1.0f, 1.0f, 1.0f };

		const glm::vec2 sizeClick = { 105, 105 };
		const glm::vec4 colClick = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec3 buttonPos = { Application::GetWidth() - 120, 110, 0.8f };

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

		constexpr int nEscapeKey = 256;
		m_BackButton.SetOptionalKey(nEscapeKey);
	}

#if 1
	//Restart button
	{
		const glm::vec2 sizeDefault = { 105,105 };
		const glm::vec4 colDefault = { 1.0f, 1.0f, 1.0f, 1.0f };

		const glm::vec2 sizeClick = { 105, 105 };
		const glm::vec4 colClick = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec3 buttonPos = { Application::GetWidth() - 270, 110, 0.8f };

		ButtonProps propDefault;
		propDefault.SetBasicProps(UITypes::ButtonRestart, sizeDefault, colDefault);
		propDefault.SetTextProps("", { 40,-20 }, 0.5, { 1,1,1,1 });

		ButtonProps propClick;
		propClick.SetBasicProps(UITypes::ButtonRestart_S, sizeClick, colClick);
		propClick.SetTextProps("", { 40,-20 }, 0.5, { 1,1,1,1 });

		m_restartButton.SetStateProperties(propDefault, Button::StateDefault);
		m_restartButton.SetStateProperties(propClick, Button::StateSelected);
		m_restartButton.SetStateProperties(propClick, Button::StateClicked);

		m_restartButton.SetPosition(buttonPos);
		m_restartButton.SetButttonClickEvent([](void* userData) {
			Application::GetCurrentApp()->SetNextMenu(Menu::None, userData);
			});

		constexpr int nRestart = 'R';	//glfw returns ascii value of the capital letter even if caps lock is turned off
		m_restartButton.SetOptionalKey(nRestart);
		m_restartButton.SetIsActive(false);
	}
#endif

	//Get the fade out layer to play an animation when the player dies
	const std::vector<Layer*>& layers = Application::GetCurrentApp()->GetLayers();
	for (Layer* pLayer : layers)
	{
		FadeoutScreenLayer* pFadeLayer = dynamic_cast <FadeoutScreenLayer*> (pLayer);
		if (pFadeLayer)
		{
			m_pFadeoutLayer = pFadeLayer;
			break;
		}
	}

	if (m_pFadeoutLayer)
	{
		m_pFadeoutLayer->SetAlphaRange({ 0.0f, 0.7f });
		m_pFadeoutLayer->SetAnimationTime(0.9f);
		m_pFadeoutLayer->SetColor({0.0f, 0.0f, 0.0f});

		//m_pFadeoutLayer->SetOptionalWaitTimeBeforeCallback(0.8f);
		//m_pFadeoutLayer->SetOptionalCallback([](void * userData)
		//	{
		//		LOG_INFO("Testing Calback");
		//	});
	}
	else
	{
		LOG_WARN("Fade out layer was not found");
	}
}

void NormalPlayerLayer::OnUpdate(float deltaTime)
{
	if (m_bIsAlive)
	{
		AbstractPlayerLayer::OnUpdate(deltaTime);

		if (!m_bIsColliding && Application::GetGameTime() > m_dNextCollideTime)
		{
			m_bCanCollide = true;
			m_vCol = { 0.5, 0.4, 0.8,1.0 };
		}
		
		//Score
		m_dScore += deltaTime;

		char buffer[10];
		std::snprintf(buffer, 10, "%.1f", m_dScore);
		Renderer::DrawTextColor(buffer, 10, m_vScorePos, m_fScoreScale, m_vScoreCol);

		//Render player
		RendererShapes::Rectangle(m_Vertex, m_vPos, m_vSize, m_vCol);
		Renderer::DrawQuadColor(m_Vertex, RendererShapes::ShapeQuad);
	}
	else
	{
		const glm::vec2 textPos = glm::vec2(500, 670);
		constexpr float textScale = 0.8f;
		const glm::vec4 textCol = { 1.0f, 1.0f, 1.0f, 1.0f };

		//Player has died, Only show the score
		char buffer[20];
		std::snprintf(buffer, 20, "Score: %.1f", m_dScore);
		Renderer::DrawTextColor(buffer, 20, textPos, textScale, textCol);
	}
}

void NormalPlayerLayer::TakeDamage(double damage)
{
	if (m_bCanCollide)
	{
		m_bCanCollide = false;
		m_bIsColliding = true;
		m_vCol = {1.0f,230.0/255, 0.0f, 0.65f };
		//Take damage

		
	}
}
void NormalPlayerLayer::TakeNoDamage()
{
	if (m_bIsColliding)
	{
		m_bIsColliding = false;
		m_dNextCollideTime = Application::GetGameTime() + m_dImmunityTime;
	}
}

void NormalPlayerLayer::Die()
{
	m_bIsAlive = false;
	if (m_pFadeoutLayer)
	{
		m_pFadeoutLayer->BeginFading();
	}
	m_restartButton.SetIsActive(true);
}

#if 1
void NormalPlayerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	AbstractPlayerLayer::RegisterEvents(pApp, nIndex);
	pApp->RegisterEvents(LayerKeyDown, nIndex);
}
bool NormalPlayerLayer::OnKeyDown(int key)
{
	if (key == 32)
	{
		LOG_INFO("Test");
		Die();
	}
	return false;
}
#endif
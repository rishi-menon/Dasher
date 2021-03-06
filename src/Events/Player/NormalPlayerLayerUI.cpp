#include "NormalPlayerLayerUI.h"
#include "Log.h"
#include "Application/Application.h"
#include "Renderer/Renderer.h"

NormalPlayerLayerUI::Data::Data() :
	pbIsAlive (nullptr),
	pdScore(nullptr),
	pnLivesUsed(nullptr),
	pfPhaseTimeRemaining(nullptr),
	pdPhaseTimeTotal(nullptr)
{
}
void NormalPlayerLayerUI::Data::SetDataNormalPlayerLayer(NormalPlayerLayer* playerLayer)
{
	pbIsAlive = &playerLayer->m_bIsAlive;
	pdScore = &playerLayer->m_dScore;
	pnLivesUsed = &playerLayer->m_nLivesUsed;
	pfPhaseTimeRemaining = &playerLayer->m_dPhaseTimeRemaining;
	pdPhaseTimeTotal = &playerLayer->m_dTotalPhaseTime;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

NormalPlayerLayerUI::NormalPlayerLayerUI() :
	//constants
	m_vHealthUISize(80, 80),
	m_fHealthUIOffsetX(90.0f),

	m_vScorePos(0.0f, 0.0),

	m_vPhaseTimeBaseSize(0.0f, 50.0f),	//Size X is calculated dynamically (But size y is needed)
	m_vPhaseTimeBasePos(0.0f, 0.0f, 0.0f)
{
	
}

void NormalPlayerLayerUI::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
}


void NormalPlayerLayerUI::OnStart()
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

	//Restart button
	//Note: the tutorial also uses this layer and so technically a restart button would be created but never shown as the player will not die. This is a hack for now
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
		m_restartButton.SetIsActive(false);	//Shouldn't be on initially
	}

	const std::vector<Layer*>& layers = Application::GetCurrentApp()->GetLayers();

	//Gets set automatically for the player layer. The tutorial layer will explicily set it on its own
	for (Layer* pLayer : layers)
	{
		NormalPlayerLayer* pPlayerLayer = dynamic_cast <NormalPlayerLayer*> (pLayer);
		if (pPlayerLayer)
		{
			m_data.SetDataNormalPlayerLayer(pPlayerLayer);
			break;
		}
	}

	//Health UI
	const glm::vec4 colHealthUI = { 1.0f, 1.0f, 1.0f, 1.0f };
	float healthUIStartingPosX = Application::GetWidth() - m_vHealthUISize.x / 2 - (NormalPlayerLayer::PlayerLives - 1) * m_fHealthUIOffsetX - 40;
	
	glm::vec3 posHealthUI = { healthUIStartingPosX, Application::GetHeight() - 200, 0.5 };	//The health ui disappears when the player dies because the z value is less than 0.7 (which is the z value of the fade out layer)
	for (int i = 0; i < NormalPlayerLayer::PlayerLives; i++)
	{
		m_HealthUI[i].SetTextureId(StandardTexture::LifeFull);
		m_HealthUI[i].SetProperties(posHealthUI, m_vHealthUISize, colHealthUI);
		posHealthUI.x += m_fHealthUIOffsetX;
	}
	

	//Phase time bar
	//For the UI, the x position is the center of the image instead of the left edge
	//For the UI, the y pos is the center. Also, we need the y coordinate of the bottom left corner so subtract the height
	m_vPhaseTimeBasePos = { healthUIStartingPosX - m_vHealthUISize.x / 2,
		posHealthUI.y - m_vHealthUISize.y/2 - m_vPhaseTimeBaseSize.y - 10, 0.5 };
	m_vPhaseTimeBaseSize.x = Application::GetWidth() - m_vPhaseTimeBasePos.x - 20;

	constexpr float fColGrayPhaseBarBase = 0.65f;
	const glm::vec4 colPhaseBarBase(fColGrayPhaseBarBase, fColGrayPhaseBarBase, fColGrayPhaseBarBase, 1.0f);
	RendererShapes::RectangleBottomLeft(m_vertexPhaseTimeBase, m_vPhaseTimeBasePos, m_vPhaseTimeBaseSize, colPhaseBarBase);
	
	SetLifeUI();	//enable/disable the life and show the correct life count

	NormalPlayerLayerUI::OnWindowResize(Application::GetWidth(), Application::GetHeight());

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//																			 //
//																			 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void NormalPlayerLayerUI::OnUpdate(float deltaTime)
{
	if (!m_data.pbIsAlive) { return; }

	if (*m_data.pbIsAlive)
	{
		//render score
		if (m_data.pdScore && *m_data.pdScore >= 0)
		{
			constexpr float textScale = 0.7f;
			const glm::vec4 textCol = { 1.0f, 1.0f, 1.0f, 1.0f };

			char buffer[10];
			std::snprintf(buffer, 10, "%.1f", *m_data.pdScore);
			Renderer::DrawTextColor(buffer, 10, m_vScorePos, textScale, textCol);
		}

		//Show phase time bar 
		DrawPhaseTimeRemainingBar();
	}
	else
	{
		//player is dead.. Show the score
		if (m_data.pdScore && *m_data.pdScore >= 0)
		{
			constexpr float textScale = 0.8f;
			const glm::vec4 textCol = { 1.0f, 1.0f, 1.0f, 1.0f };

			char buffer[20];
			std::snprintf(buffer, 20, "Score: %.1f", *m_data.pdScore);
			Renderer::DrawTextColor(buffer, 20, m_vScorePos, textScale, textCol);
		}
	}
}

void NormalPlayerLayerUI::SetLifeUI()
{
	if (!m_data.pnLivesUsed) 
	{ 
		for (int i = 0; i < NormalPlayerLayer::PlayerLives; i++)
		{
			m_HealthUI[i].SetIsActive(false);
		}
		return; 
	}

	int nCurLives = NormalPlayerLayer::PlayerLives - *m_data.pnLivesUsed;

	int i = 0;
	for (; i < nCurLives; i++)
	{
		m_HealthUI[i].SetIsActive(true);
		m_HealthUI[i].SetTextureId(StandardTexture::LifeFull);
	}
	for (; i < NormalPlayerLayer::PlayerLives; i++)
	{
		m_HealthUI[i].SetIsActive(true);
		m_HealthUI[i].SetTextureId(StandardTexture::LifeEmpty);
	}
}

void NormalPlayerLayerUI::PlayerHasDied()
{
	//Change score pos to the center of the screen
	constexpr float percentX = 500.0f / 1600.0f;
	constexpr float percentY = 670.0f / 1200.0f;
	m_vScorePos.x = (float)Math::Lerp(0, Application::GetWidth(), percentX);
	m_vScorePos.y = (float)Math::Lerp(0, Application::GetHeight(), percentY);

	m_restartButton.SetIsActive(true);
}
bool NormalPlayerLayerUI::OnWindowResize(int x, int y)
{
	{
		float posX = x - 120.0f;
		if (posX < 20) { posX = 20; }

		const glm::vec3 buttonPos = { posX, 110, 0.8f };
		m_BackButton.SetPosition(buttonPos);
	}
	{
		float posX = x - 270.0f;
		if (posX < 20) { posX = 20; }
		const glm::vec3 buttonPos = { posX, 110, 0.8f };
		m_restartButton.SetPosition(buttonPos);
	}
	
	if (!m_data.pbIsAlive || *m_data.pbIsAlive)
	{
		//if alive or null
		m_vScorePos.x = x - 280.0f;
		m_vScorePos.y = y - 140.0f;
	}
	else
	{
		constexpr float percentX = 500.0f / 1600.0f;
		constexpr float percentY = 670.0f / 1200.0f;
		m_vScorePos.x = (float)Math::Lerp(0, x, percentX);
		m_vScorePos.y = (float)Math::Lerp(0, y, percentY);
	}

	//Health UI
	const glm::vec4 colHealthUI = { 1.0f, 1.0f, 1.0f, 1.0f };
	float healthUIStartingPosX = Application::GetWidth() - m_vHealthUISize.x / 2 - (NormalPlayerLayer::PlayerLives - 1) * m_fHealthUIOffsetX - 40;
	glm::vec3 posHealthUI = { healthUIStartingPosX, Application::GetHeight() - 200, 0.5 };	//The health ui disappears when the player dies because the z value is less than 0.7 (which is the z value of the fade out layer)
	for (int i = 0; i < NormalPlayerLayer::PlayerLives; i++)
	{
		m_HealthUI[i].SetPos (posHealthUI);
		posHealthUI.x += m_fHealthUIOffsetX;
	}
	

	//Phase time bar
	//For the UI, the x position is the center of the image instead of the left edge
	//For the UI, the y pos is the center. Also, we need the y coordinate of the bottom left corner so subtract the height
	m_vPhaseTimeBasePos = { healthUIStartingPosX - m_vHealthUISize.x / 2,
		posHealthUI.y - m_vHealthUISize.y/2 - m_vPhaseTimeBaseSize.y - 10, 0.5 };
	m_vPhaseTimeBaseSize.x = Application::GetWidth() - m_vPhaseTimeBasePos.x - 20;

	constexpr float fColGrayPhaseBarBase = 0.65f;
	const glm::vec4 colPhaseBarBase(fColGrayPhaseBarBase, fColGrayPhaseBarBase, fColGrayPhaseBarBase, 1.0f);
	RendererShapes::RectangleBottomLeft(m_vertexPhaseTimeBase, m_vPhaseTimeBasePos, m_vPhaseTimeBaseSize, colPhaseBarBase);

	return false;
}

void NormalPlayerLayerUI::DrawPhaseTimeRemainingBar()
{

	if (!m_data.pfPhaseTimeRemaining || !m_data.pdPhaseTimeTotal) { return; }
	//Base
	Renderer::DrawQuadColor(m_vertexPhaseTimeBase, RendererShapes::Shape::ShapeQuad);

	//Time Rem
	glm::vec2 size = { 0.0f, m_vPhaseTimeBaseSize.y };

	float timeRem = (float)Math::ClampLeft(0, *m_data.pfPhaseTimeRemaining);
	float phasePercent = (float)Math::GetPercent(0.0, *m_data.pdPhaseTimeTotal, timeRem);
	size.x = phasePercent * m_vPhaseTimeBaseSize.x;

	glm::vec4 col = { 0.1f, 1.0f, 0.3f, 0.8f };
	RendererVertex vertex[4];
	RendererShapes::RectangleBottomLeft(vertex, m_vPhaseTimeBasePos, size, col);
	Renderer::DrawQuadColor(vertex, RendererShapes::Shape::ShapeQuad);
}

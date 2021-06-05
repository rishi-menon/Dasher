#include "TutorialBlockSpawnerLayer.h"
#include "Application/Application.h"
#include "Events/Tutorial/TutorialPlayerLayer.h"
#include "Renderer/Renderer.h"
#include "BlockSpawnerFunc/TutorialSpawnerFunc.h"

TutorialStageProps::TutorialStageProps(const char* str, const glm::vec2& pos, float scale, const glm::vec4& col) :
	strText (str),
	strPos (pos),
	strScale(scale),
	strTextCol (col)
{

}

//////////////////////////////////////////
TutorialBlockSpawnerLayer::TutorialBlockSpawnerLayer() :
	m_pTutorialPlayer (nullptr),
	m_pBackgroundLayer (nullptr), 

	m_CurStage(TutorialStage::TextIntro),

	m_dNextStageSwitchTime(0.0f),
	m_bStageOver (false)
{
	if (!g_currentInstance)
	{
		g_currentInstance = this;
	}
	else
	{
		LOG_WARN("Two tutorials layers were created");
	}

	m_mapStages.reserve(20);
	m_mapWaitTime.reserve(10);

	m_CreateBlockFunc = TutorialSpawnerFunc;
	CreateStages();
}

TutorialBlockSpawnerLayer::~TutorialBlockSpawnerLayer()
{
	g_currentInstance = nullptr;
	m_mapStages.clear();
}

void TutorialBlockSpawnerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	BlockSpawnerLayer::RegisterEvents(pApp, nIndex);
	pApp->RegisterEvents(LayerWindowResize, nIndex);
	pApp->RegisterEvents(LayerMouseUp, nIndex);
	pApp->RegisterEvents(LayerKeyUp, nIndex);
	
}

void TutorialBlockSpawnerLayer::OnStart()
{
	BlockSpawnerLayer::OnStart();

	const std::vector<Layer*>& layers = Application::GetCurrentApp()->GetLayers();
	for (Layer* pLayer : layers)
	{
		TutorialPlayerLayer* playerLayer = dynamic_cast<TutorialPlayerLayer*>(pLayer);
		if (playerLayer)
		{
			m_pTutorialPlayer = playerLayer;
			break;
		}
	}
	for (Layer* pLayer : layers)
	{
		BackgroundLayer* backgroundLayer = dynamic_cast<BackgroundLayer*>(pLayer);
		if (backgroundLayer)
		{
			m_pBackgroundLayer = backgroundLayer;
			break;
		}
	}

	ASSERT(m_pTutorialPlayer && m_pBackgroundLayer, "The tutorial layers were not found");

	StartCurrentStage();
}

void TutorialBlockSpawnerLayer::OnUpdate(float deltaTime)
{
	if (!m_pTutorialPlayer) { return; }

	if (m_CurStage >= TutorialStage::Count)
	{
		m_pTutorialPlayer->TutorialOver();
		return;
	}

	if (m_CurIterator == m_mapStages.end())
	{
		//Playable stage
		BlockSpawnerLayer::OnUpdate(deltaTime);

		if (!m_bStageOver)
		{
			//stage is not yet over
			if (m_CurStage == TutorialStage::PlayPhasing)
			{
				//Draw gray rectangles to indicate the phase region
				ShowPhaseRange();
			}

			if (!GetBlockCount())
			{
				//Player has passed through all the blocks, Stage is over (Start the wait time).
				m_bStageOver = true;
				float waitTime;
				std::unordered_map<TutorialStage, float>::iterator it = m_mapWaitTime.find(m_CurStage);
				if (it != m_mapWaitTime.end())
				{
					waitTime = it->second;
				}
				else
				{
					ASSERT(false, "Wait time does not exist in the map");
					waitTime = 3.0;
				}
				m_dNextStageSwitchTime = (float)(Application::GetGameTime()) + waitTime;
			}
		}
		else
		{
			//stage is over, waiting to begin the next stage
			if (Application::GetGameTime() > m_dNextStageSwitchTime)
			{
				//You can advance to the next stage
				m_CurStage = (TutorialStage)((int)m_CurStage + 1);
				StartCurrentStage();
			}
		}
	}
	else
	{
		//Text stage
		const TutorialStageProps& props = m_CurIterator->second;
		Renderer::DrawTextColor(props.strText, 30000, props.strPos, props.strScale, props.strTextCol);
	}
}

bool TutorialBlockSpawnerLayer::OnMouseMove(int x, int y)
{
	return BlockSpawnerLayer::OnMouseMove(x, y);
}
	 
bool TutorialBlockSpawnerLayer::OnMouseUp(int nButton)
{
	constexpr int nMouseButtonLeft = 0;

	if (nButton == nMouseButtonLeft)
	{
		if (m_CurIterator != m_mapStages.end() || m_CurStage == TutorialStage::PlayMove)
		{
			m_CurStage = (TutorialStage)((int)m_CurStage + 1);
			StartCurrentStage();
		}

	}
	return false;
}
bool TutorialBlockSpawnerLayer::OnKeyUp(int key)
{
	constexpr int nEnterKey1 = 257;
	constexpr int nEnterKey2 = 335;

	if (key == nEnterKey1 || key == nEnterKey2)
	{
		if (m_CurIterator != m_mapStages.end() || m_CurStage == TutorialStage::PlayMove)
		{
			//advance to the next if its a text field or if its the first play field (first play field doesnt have any blocks and can be skipped if the player decides to do it)
			m_CurStage = (TutorialStage)((int)m_CurStage + 1);
			StartCurrentStage();
		}
	}
	return false;
}

void TutorialBlockSpawnerLayer::StartCurrentStage()
{
	m_bStageOver = false;
	GetBlocks().ClearAll();
	if (m_CurStage >= TutorialStage::Count)
	{
		return;
	}

	m_pTutorialPlayer->StartStage(m_CurStage);

	m_CurIterator = m_mapStages.find(m_CurStage);
	if (m_CurIterator == m_mapStages.end())
	{
		//Playable Stage
		m_pTutorialPlayer->SetPauseTutorial(false);
		if (m_CurStage != TutorialStage::PlayMove)
		{
			SetNextSpawnTime(0.0);	//Spawn immediately
		}
	}
	else
	{
		m_CurIterator->second.backgroundProps = m_pBackgroundLayer->GetBackgroundProp();
		m_pTutorialPlayer->SetPauseTutorial(true);
		SetNextSpawnTime(-10.0);	//never spawn
	}
}

void TutorialBlockSpawnerLayer::ShowPhaseRange()
{
#ifdef RM_DEBUG
	if (!m_pTutorialPlayer) { ASSERT(false, "Tutorial player layer does not exist in ShowPhaseRange"); return; }
#endif

	glm::vec3 playerPos = m_pTutorialPlayer->GetPlayerPos();
	CircularQueue<Block>& blocks = GetBlocks();
	int nIndex = blocks.Begin();
	bool bFound = false;
	for (std::size_t i = 0; i < blocks.Count(); i++)
	{
		if (blocks.Buffer()[nIndex].position.x > playerPos.x)
		{
			bFound = true;
			break;
		}
		nIndex = (nIndex + 1) % blocks.Size();
	}
	if (bFound)
	{
		glm::vec4 col = { 0.2, 1.0, 0.2, 0.4f };
		glm::vec2 range = blocks.Buffer()[nIndex].phaseRange;

		float posX = range.x * Application::GetWidth();
		float width = (range.y - range.x) * Application::GetWidth();

		RendererVertex vertex[4];
		RendererShapes::RectangleBottomLeft(vertex, { posX, 0.0, 0.0 }, { width, Application::GetHeight() }, col);
		Renderer::DrawQuadColor(vertex, RendererShapes::Shape::ShapeQuad);
	}
	
}

void TutorialBlockSpawnerLayer::RestartCurrentStage()
{
	MapStages::iterator it = m_CurIterator;
	
	//find a valid text stage before this play stage (if it is a play stage, else it just gets ignored)
	for (int i = (int)m_CurStage; i >= 0 && it == m_mapStages.end(); i--)
	{
		it = m_mapStages.find((TutorialStage)(i));
	}
	if (it != m_mapStages.end())
	{
		m_pBackgroundLayer->SetBackgroundState(it->second.backgroundProps);
	}
	StartCurrentStage();
}
void TutorialBlockSpawnerLayer::CreateStages()
{
	const glm::vec4 textCol = {0.9f, 0.9f, 0.9f, 1.0f};
	TutorialStageProps props = TutorialStageProps(
		"Hi there!\n"
		"You can press ESC or the back button to skip\n"
		"this tutorial or you can press the left mouse\n"
		"button or the enter key to proceed", glm::vec2(100, 1000), 0.4f, textCol);

	m_mapStages.emplace (TutorialStage::TextIntro, props);

	props = TutorialStageProps(
		"You will constantly oscillate up and down\n"
	    "Move the mouse cursor left and right to control\n" 
		"the speed at which you move\n"
		"Give it a try!", glm::vec2(100, 1000), 0.4f, textCol);
	m_mapStages.emplace (TutorialStage::TextBasicMove, props);

	m_mapWaitTime.emplace(TutorialStage::PlayMove, 7.0f);

	props = TutorialStageProps(
		"Awesome! Now try dodging these very friendly spikes\n"
		"that are thrown at you\n", glm::vec2(100, 1000), 0.4f, textCol);
	m_mapStages.emplace(TutorialStage::TextFriendlySpikes, props);

	m_mapWaitTime.emplace(TutorialStage::PlayFriendlySpikes, 0.9f);

	props = TutorialStageProps(
		"Alrighty, now onto Phasing!\n"
		"If you move at the just the right speed, then the spike\n"
		"becomes transparent and you can safely pass through\n"
		"it without taking any damage. Each spike has a\n"
		"different \'speed\' requirement.\n\n"
		"Phasing consumes energy which replenishes\n"
		"over time", glm::vec2(100, 1000), 0.4f, textCol);
	m_mapStages.emplace(TutorialStage::TextPhasing, props);

	m_mapWaitTime.emplace(TutorialStage::PlayPhasing, 0.9f);
	
	props = TutorialStageProps("Awesome! You\'re a natural at phasing!", glm::vec2(100, 1000), 0.4f, textCol);
	m_mapStages.emplace(TutorialStage::TextTutorialOver, props);

	OnWindowResize(Application::GetWidth(), Application::GetHeight());	//Set the position of the tutorial text fields
}

bool TutorialBlockSpawnerLayer::OnWindowResize (int x, int y) 
{
	BlockSpawnerLayer::OnWindowResize(x, y);
	int nHeight = Application::GetHeight();
	//glm::vec2(100, 1000)

	for (auto& pair : m_mapStages)
	{
		TutorialStageProps& props = pair.second;
		props.strPos = { 100.0f, nHeight * 1000.0f / 1200.0f };
	}
#if 0
	m_mapStages.at(TutorialStage::TextIntro).strPos = 
	m_mapStages.at(TutorialStage::TextBasicMove).strPos = {100.0f, nHeight*1000.0f/1200.0f};
	m_mapStages.at(TutorialStage::TextFriendlySpikes).strPos = {100.0f, nHeight*1000.0f/1200.0f};
	//m_mapStages.at(TutorialStage::TextWarningCollision).strPos = {100.0f, nHeight*1000.0f/1200.0f};
	m_mapStages.at(TutorialStage::TextPhasing).strPos = {100.0f, nHeight*1000.0f/1200.0f};
	m_mapStages.at(TutorialStage::TextTutorialOver).strPos = {100.0f, nHeight*1000.0f/1200.0f};
#endif
	return false;
}
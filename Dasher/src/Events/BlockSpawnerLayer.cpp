
#include "BlockSpawnerLayer.h"
#include "Application/Application.h"

#include "AssetManagement/Texture.h"
#include "Renderer/Renderer.h"

#include "Collision/Collision.h"
#include "Events/Player/AbstractPlayerLayer.h"


BlockSpawnerLayer::BlockSpawnerLayer() :
	m_pPlayerLayer (nullptr),
	m_dNextSpawnTime (0.0),

	m_dTimeBwSpawnMin(1.4),
	m_dTimeBwSpawnMax (2.2),
	m_dSizeXMin (80),
	m_dSizeXMax (100),
	m_dSizeYMin (300),
	m_dSizeYMax (500),
	m_bPreviousCollided (false)
{
	
	m_blocks.Reserve(30);
	m_blocks.ClearAll();
	m_dNextSpawnTime = Application::GetGameTime();
}
void BlockSpawnerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
	pApp->RegisterEvents(LayerMouseMove, nIndex);
}

void BlockSpawnerLayer::OnStart()
{
	const std::vector<Layer*>& layers = Application::GetCurrentApp()->GetLayers();
	for (Layer* pLayer : layers)
	{
		if (dynamic_cast<AbstractPlayerLayer*> (pLayer))
		{
			m_pPlayerLayer = static_cast<AbstractPlayerLayer*>(pLayer);
			break;
		}
	}
	ASSERT(m_pPlayerLayer, "Player layer was not found");
}

void BlockSpawnerLayer::ClearOutOfBoundsBlocks()
{
	//Check if the first block in the queue is out of bounds. Blocks move at the same speed so the first one in the queue is the only one that needs to be checked instead of every single block
	if (m_blocks.Count())
	{
		Block* block = m_blocks.GetAtRaw(m_blocks.Begin());
		if (block->position.x + block->scale.x < 0)
		{
			//the block has gone sufficiently out of the screen 
			m_blocks.Pop();
		}
	}
}
void BlockSpawnerLayer::OnUpdate(float deltaTime)
{
	////////////////////////////////////////////////////////////////////////////////////
	//								Game Logic										  //
	//		1. Clear out of bounds blocks											  //
	//		2. For each existing block												  //
	//				- Update position												  //
	//				- Check collision with player									  //
	//				- Render														  //
	//		3. Create new blocks if necessary										  //
	////////////////////////////////////////////////////////////////////////////////////

	//1. Clear out of bound blocks
	ClearOutOfBoundsBlocks();
	
	//2.
	MoveCollisionRenderBlocks(deltaTime);

	//3.Add new blocks 
	double dCurTime = Application::GetGameTime();	//in seconds
	if (dCurTime >= m_dNextSpawnTime)
	{
		m_dNextSpawnTime = dCurTime + Random::Rand(m_dTimeBwSpawnMin, m_dTimeBwSpawnMax);
		CreateBlock();
	}
}

void BlockSpawnerLayer::MoveCollisionRenderBlocks(float deltaTime)
{
	const RendererVertex* playerVertex = m_pPlayerLayer->GetVertex();
	constexpr unsigned int nPlayerVertexCount = (unsigned int)AbstractPlayerLayer::GetVertexCount();

	//This is for optimization, We dont have to check collision with every single block. Since its a queue, if the blocks position is more than the players x position, then we can skip collision for all the blcoks after that
	float playerMaxPosX = -1000;
	for (unsigned int i = 0; i < nPlayerVertexCount; i++)
	{
		playerMaxPosX = glm::max(playerMaxPosX, playerVertex[i].m_pos.x);
	}

	Block* buffer = m_blocks.Buffer();
	int index = m_blocks.Begin();

	bool playerCollided = false;
	bool checkCollision = true;
	for (unsigned int i = 0; i < m_blocks.Count(); i++)
	{
		//Update the block's position
		Block& curBlock = buffer[index];
		curBlock.position += curBlock.velocity * deltaTime;

		RendererVertex blockVertex[3];
		RendererShapes::TriangleRegular(blockVertex, curBlock.position, curBlock.scale, curBlock.color);

		//Check Collision if need be
		if (checkCollision)
		{
			float minBlockPosX = 100000;
			for (unsigned int i = 0; i < RendererShapes::genericVertexBufferCount[curBlock.shape]; i++)
			{
				minBlockPosX = glm::min(minBlockPosX, blockVertex[i].m_pos.x);
			}

			if (minBlockPosX > playerMaxPosX)
			{
				checkCollision = false;
			} 
			else if (!curBlock.isPhasable && Collision::CheckCollision(playerVertex, nPlayerVertexCount, blockVertex, 3))
			{
				playerCollided = true;
				m_bPreviousCollided = true;
				m_pPlayerLayer->TakeDamage(10);
			}
		}

		//Render
		Renderer::DrawQuadColor(blockVertex, curBlock.shape);
		index = (index + 1) % m_blocks.Size();
	}

	if (m_bPreviousCollided && !playerCollided)
	{
		m_bPreviousCollided = false;
		m_pPlayerLayer->TakeNoDamage();
	}
}
void BlockSpawnerLayer::CreateBlock()
{
	Block& block = *m_blocks.Push();

	if (Random::Rand() >= 0.5)
	{
		//Block Top
		block.position = { Application::GetWidth(), Application::GetHeight() - 2, 0 };
		//Negative sign so that the triangle appears upside down
		block.scale = { Random::Rand(m_dSizeXMin,m_dSizeXMax), -Random::Rand(m_dSizeYMin,m_dSizeYMax), 1 };
	}
	else
	{
		//block bottom
		Application* pApp = Application::GetCurrentApp();
		block.position = { pApp->GetWidth(), 2, 0 };
		block.scale = { Random::Rand(m_dSizeXMin,m_dSizeXMax), Random::Rand(m_dSizeYMin,m_dSizeYMax), 1 };
	}

	block.velocity = { -400, 0, 0 };
	block.color = { 0.0,0.4,0.79,1.0 };
	block.shape = RendererShapes::ShapeTriangleRegular;
	block.phaseRange = { 0.0, 0.2 };


	double curPhase = m_pPlayerLayer->GetPhasePercent();
	if (block.phaseRange.x <= curPhase && block.phaseRange.y >= curPhase)
	{
		block.isPhasable = true;
		block.color.a = 0.5f;
	}
	else
	{
		block.isPhasable = false;
		block.color.a = 1.0f;
	}	
}


bool BlockSpawnerLayer::OnMouseMove(int x, int y)
{
	if (!m_pPlayerLayer) { ASSERT(false, "PlayerLayer was null"); return false; }

	//Recalculate phasable blocks
	int index = m_blocks.Begin();
	Block* buffer = m_blocks.Buffer();

	double curPhase = m_pPlayerLayer->GetPhasePercent();
	for (unsigned int i = 0; i < m_blocks.Count(); i++)
	{
		Block& curBlock = buffer[index];
		if (curBlock.phaseRange.x <= curPhase && curBlock.phaseRange.y >= curPhase)
		{
			curBlock.isPhasable = true;
			curBlock.color.a = 0.5f;
		}
		else
		{
			curBlock.isPhasable = false;
			curBlock.color.a = 1.0f;
		}
		index = (index + 1) % m_blocks.Size();
	}
	return false;
}

#if 0
bool BlockSpawnerLayer::OnWindowResize(int width, int height)
{
	return false;
}
#endif
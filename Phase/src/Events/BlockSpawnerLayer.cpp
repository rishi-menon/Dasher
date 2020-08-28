
#include "BlockSpawnerLayer.h"
#include "Application/Application.h"

#include "AssetManagement/Texture.h"
#include "AssetManagement/StandardTexture.h"
#include "Renderer/Renderer.h"

#include "Collision/Collision.h"
#include "Events/Player/AbstractPlayerLayer.h"

#include "BlockSpawnerFunc/DefaultSpawnerFunc.h"

#if 0
static void CreateBlockDefault(Block& newBlock, double& nextSpawnTime, double curPhase)
{
	//spawn blocks
	const glm::vec2 timeBwSpawn = { 1.4f, 2.2f };

	//size
	const glm::vec2 sizeX = { 80, 100 };
	const glm::vec2 sizeY = { 300, 500 };

	if (Random::Rand() >= 0.5)
	{
		//newBlock Top
		newBlock.position = { Application::GetWidth(), Application::GetHeight() - 2, 0.1 };
		//Negative sign so that the triangle appears upside down
		newBlock.scale = { Random::Rand(sizeX.x, sizeX.y), -Random::Rand(sizeY.x, sizeY.y), 1 };
	}
	else
	{
		//newBlock bottom
		Application* pApp = Application::GetCurrentApp();
		newBlock.position = { pApp->GetWidth(), 2, 0.1 };
		newBlock.scale = { Random::Rand(sizeX.x, sizeX.y), Random::Rand(sizeY.x, sizeY.y), 1 };
	}

	newBlock.velocity = { -400, 0, 0 };
	newBlock.color = { 0.0,0.4,0.79,1.0 };
	newBlock.shape = RendererShapes::ShapeTriangleRegular;
	newBlock.phaseRange = { 0.0, 0.2 };

	if (newBlock.phaseRange.x <= curPhase && newBlock.phaseRange.y >= curPhase)
	{
		newBlock.isPhasable = true;
		newBlock.color.a = 0.5f;
	}
	else
	{
		newBlock.isPhasable = false;
		newBlock.color.a = 1.0f;
	}
	nextSpawnTime = Application::GetGameTime() + Random::Rand(timeBwSpawn.x, timeBwSpawn.y);
}
#endif

BlockSpawnerLayer::BlockSpawnerLayer() :
	m_pPlayerLayer (nullptr),
	m_dNextSpawnTime (0.0),

	m_bPreviousCollided (false),
	m_dCurPhasePercent(0)
{
	m_blocks.Reserve(30);
	m_blocks.ClearAll();
	m_dNextSpawnTime = Application::GetGameTime();

	m_CreateBlockFunc = DefaultSpawnerFunc;
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
	if (!m_pPlayerLayer)
	{
		LOG_WARN("Player layer was not found");
	}

	//To Do: make this layer independent of the player layer maybe ? So if player layer does not exist then this class should still work ??
	ASSERT(m_pPlayerLayer, "Player layer was not found");

	m_nBlockTextureId = GetStandardTextureId(StandardTexture::SpecialEffect0);
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
	//Added the second condition so that we can force it to never spawn by setting it to a negative number. This will be useful in the tutorial layer
	if (dCurTime >= m_dNextSpawnTime && m_dNextSpawnTime >= 0.0)	
	{
		m_CreateBlockFunc(m_blocks, m_dNextSpawnTime, m_dCurPhasePercent);
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
		//Renderer::DrawQuadColor(blockVertex, curBlock.shape);
		Renderer::DrawQuadTexture(blockVertex, curBlock.shape, m_nBlockTextureId);
		index = (index + 1) % m_blocks.Size();
	}

	if (m_bPreviousCollided && !playerCollided)
	{
		m_bPreviousCollided = false;
		m_pPlayerLayer->TakeNoDamage();
	}
}

bool BlockSpawnerLayer::OnMouseMove(int x, int y)
{
	if (!m_pPlayerLayer) { ASSERT(false, "PlayerLayer was null"); return false; }

	//Recalculate phasable blocks
	int index = m_blocks.Begin();
	Block* buffer = m_blocks.Buffer();

	m_dCurPhasePercent = Math::GetPercent(0, Application::GetWidth(), x);

	for (unsigned int i = 0; i < m_blocks.Count(); i++)
	{
		Block& curBlock = buffer[index];
		if (curBlock.phaseRange.x <= m_dCurPhasePercent && curBlock.phaseRange.y >= m_dCurPhasePercent)
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

void BlockSpawnerLayer::RenderBlocks()
{
	unsigned int index = m_blocks.Begin();
	Block* buffer = m_blocks.Buffer();

	for (unsigned int i = 0; i < m_blocks.Count(); i++)
	{
		Block& curBlock = buffer[index];
		RendererVertex blockVertex[3];
		RendererShapes::TriangleRegular(blockVertex, curBlock.position, curBlock.scale, curBlock.color);

		//Render
		//Renderer::DrawQuadColor(blockVertex, curBlock.shape);
		Renderer::DrawQuadTexture(blockVertex, curBlock.shape, m_nBlockTextureId);
		index = (index + 1) % m_blocks.Size();
	}
}


#include "BlockSpawnerLayer.h"
#include "Application/Application.h"

#include "AssetManagement/Texture.h"
#include "AssetManagement/StandardTexture.h"
#include "Renderer/Renderer.h"

#include "Collision/Collision.h"
#include "Events/Player/AbstractPlayerLayer.h"

#include "BlockSpawnerFunc/DefaultSpawnerFunc.h"

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
			else if (Collision::CheckCollision(playerVertex, nPlayerVertexCount, blockVertex, 3))
			{
				playerCollided = true;
				m_bPreviousCollided = true;
				if (!curBlock.isPhasable)
				{
					
					m_pPlayerLayer->TakeDamage(10);
				}
				else
				{
					m_pPlayerLayer->TakePhaseDamage();
				}
			}
		}

		//Render
		Renderer::DrawQuadTexture(blockVertex, curBlock.shape, m_nBlockTextureId);
		index = (index + 1) % m_blocks.Size();
	}

	if (m_bPreviousCollided && !playerCollided)
	{
		m_bPreviousCollided = false;
		m_pPlayerLayer->TakeNoDamage();
	}
}

void BlockSpawnerLayer::RecalculateBlockPhase(float mousePosX)
{
	int index = m_blocks.Begin();
	Block* buffer = m_blocks.Buffer();

	if (!m_pPlayerLayer) { ASSERT(false, "PlayerLayer was null"); return; }

	bool bPlayerCanPhase = m_pPlayerLayer->CanPhase();

	if (bPlayerCanPhase)
	{
		m_dCurPhasePercent = Math::GetPercent(0, Application::GetWidth(), mousePosX);
	}
	else
	{
		m_dCurPhasePercent = -1;	//invalid so that new blocks that are created will never be phasable
	}
	for (unsigned int i = 0; i < m_blocks.Count(); i++)
	{
		Block& curBlock = buffer[index];
		if (bPlayerCanPhase && curBlock.phaseRange.x <= m_dCurPhasePercent && curBlock.phaseRange.y >= m_dCurPhasePercent)
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
}
bool BlockSpawnerLayer::OnMouseMove(int x, int y)
{
	//Recalculate phasable blocks
	RecalculateBlockPhase(x);
	return false;
}

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

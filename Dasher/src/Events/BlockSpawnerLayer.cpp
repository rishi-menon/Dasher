
#include "BlockSpawnerLayer.h"
#include "Application/Application.h"

#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

#include "Collision/Collision.h"
#include "PlayerLayer.h"

BlockSpawnerLayer::BlockSpawnerLayer() :
	m_pPlayerLayer (nullptr),
	m_dNextSpawnTime (0.0),

	m_dTimeBwSpawnMin(1.4),
	m_dTimeBwSpawnMax (2.2),
	m_dSizeXMin (80),
	m_dSizeXMax (100),
	m_dSizeYMin (300),
	m_dSizeYMax (500)
{
	
	m_blocks.Reserve(30);
	BlockSpawnerLayer::ResetLayer();
}
void BlockSpawnerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
	pApp->RegisterEvents(LayerMouseMove, nIndex);
}
void BlockSpawnerLayer::ResetLayer()
{
	m_blocks.ClearAll();
	m_dNextSpawnTime = Application::GetGameTime();
}

void BlockSpawnerLayer::OnStart()
{
	const std::vector<Layer*>& layers = Application::GetCurrentApp()->GetLayers();
	for (Layer* pLayer : layers)
	{
		if (dynamic_cast<PlayerLayer*> (pLayer))
		{
			m_pPlayerLayer = static_cast<PlayerLayer*>(pLayer);
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
	//																				  //
	//		1. Clear out of bounds blocks											  //
	//		2. For each existing block												  //
	//				- Update position												  //
	//				- Check collision with player									  //
	//				- Render														  //
	//		3. Create new blocks if necessary										  //
	////////////////////////////////////////////////////////////////////////////////////

	//1. Clear out of bound blocks
	ClearOutOfBoundsBlocks();
	
	const RendererVertex* playerVertex = m_pPlayerLayer->GetVertex();
	constexpr unsigned int nPlayerVertexCount = (unsigned int)PlayerLayer::GetVertexCount();
	Block* buffer = m_blocks.Buffer();
	int index = m_blocks.Begin();

	//To do: Delete temp code
	bool TempPlayerCollision = false;

	//2. Update position, check collision and render
	for (unsigned int i = 0; i < m_blocks.Count(); i++, index++, index %= m_blocks.Size())
	{
		//Update the block's position
		Block& curBlock = buffer[index];
		curBlock.position += curBlock.velocity * deltaTime;

		RendererVertex blockVertex[3];
		RendererShapes::TriangleRegular(blockVertex, curBlock.position, curBlock.scale, curBlock.color);

		if (Collision::CheckCollision(playerVertex, nPlayerVertexCount, blockVertex, 3))
		{
			TempPlayerCollision = true;
			//To Do: player collided with a block... Do stuff....
			m_pPlayerLayer->TakeDamage(10);
			//for (int i = 0; i < 3; i++)	blockVertex[i].m_col = { 1.0, 0.2, 0.2, 1.0 };
		}
		Renderer::DrawQuadColor(blockVertex, curBlock.shape);
	}

	if (!TempPlayerCollision)
	{
		m_pPlayerLayer->TakeDamage(0);
	}

	//3.Add new blocks 
	
	double dCurTime = Application::GetGameTime();	//in seconds
	if (dCurTime >= m_dNextSpawnTime)
	{

		//To do: delete temp in future
		double temp = Random::Rand(m_dTimeBwSpawnMin, m_dTimeBwSpawnMax);
		m_dNextSpawnTime = dCurTime + temp;

		Block& block = *m_blocks.Push();

		if (Random::Rand() >= 0.5)
		{
			//Block Top
			Application* pApp = Application::GetCurrentApp();
			block.position = { pApp->GetWidth(), pApp->GetHeight()-2, 0 };
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
		block.color = { 0.0,0.4,0.79,0.5 };
		block.shape = RendererShapes::ShapeTriangleRegular;
	}
}
bool BlockSpawnerLayer::OnWindowResize(int width, int height)
{
	return false;
}

//Temp code
bool BlockSpawnerLayer::OnMouseMove(double x, double y)
{
	//if (m_blocks.Count())
	//{
	//	Block* block = m_blocks.GetAt(0);
	//	//LOG_INFO("x {0}, y{1}", x, y);
	//	block->position = { x, y, 1 };
	//}

	return false;
}
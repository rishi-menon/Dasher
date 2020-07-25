
#include "BlockSpawnerLayer.h"
#include "Application/Application.h"

#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

#include "Collision/Collision.h"
#include "PlayerLayer.h"

BlockSpawnerLayer::BlockSpawnerLayer() :
	m_pPlayerLayer (nullptr)
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


	//Temporary code
	Block& block = *m_blocks.Push ();
	block.position = { 400, 400, 0 };
	block.scale = { 40, 40, 1 };
	block.velocity = { -200, 0, 0 };
	block.color = { 0.8,0.8,0.8,1.0 };
	block.shape = RendererShapes::ShapeTriangleRegular;
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

	ClearOutOfBoundsBlocks();
	
	const RendererVertex* playerVertex = m_pPlayerLayer->GetVertex();
	constexpr unsigned int nPlayerVertexCount = (unsigned int)PlayerLayer::GetVertexCount();
	Block* buffer = m_blocks.Buffer();
	int index = m_blocks.Begin();

	//Update position, check collision and render
	for (unsigned int i = 0; i < m_blocks.Count(); i++, index++, index %= m_blocks.Size())
	{
		//Update the block's position
		Block& curBlock = buffer[index];
		curBlock.position += curBlock.velocity * deltaTime;

		RendererVertex blockVertex[3];
		RendererShapes::TriangleRegular(blockVertex, curBlock.position, curBlock.scale, curBlock.color);

		if (Collision::CheckCollision(playerVertex, nPlayerVertexCount, blockVertex, 3))
		{
			for (int i = 0; i < 3; i++)	blockVertex[i].m_col = { 1.0, 0.2, 0.2, 1.0 };
		}
		Renderer::DrawQuadColor(blockVertex, curBlock.shape);
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
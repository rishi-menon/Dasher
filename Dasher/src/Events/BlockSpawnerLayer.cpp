
#include "BlockSpawnerLayer.h"
#include "Application/Application.h"

#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

#include "Collision/Collision.h"
#include "PlayerLayer.h"

struct BlockAdditionalData
{
	RendererVertex vertex[6];
	unsigned int index[8];

	unsigned int nVertexCount;
	unsigned int nIndexCount;
	glm::vec3 vCenterStretching;	//describes the center of the shape for stretching in different directions without changing the position (ideally). Describes the origin for the stretching
	
	//To do: implement this, POSition vector should point to the bottom edge of the triangle.
	glm::vec3 vCenterPosition;		//The position vector of the block describes the location of this point
};

static BlockAdditionalData blockData[5];

static void CreateShapes()
{
	RendererVertex* curVertex;
	unsigned int* curIndex;

	//Block 0 - Right angle triangle ponting upwards
	curVertex = blockData[0].vertex;
	curIndex = blockData[0].index;

	const glm::vec4 col = { 0.8,0.8,0.8,1.0 };
	
	/*curVertex[0].SetPosCol({ 0.0, 0.0, 0.0 }, col);
	curVertex[1].SetPosCol({ 1.0, 0.0, 0.0 }, col);
	curVertex[2].SetPosCol({ 0.5, 0.866, 0.0 }, col);*/

	curVertex[0].SetPosCol({ -1.0, 0.0, 0.0 }, col);
	curVertex[1].SetPosCol({ 1.0, 0.0, 0.0 }, col);
	curVertex[2].SetPosCol({ 0.0, 1.73, 0.0 }, col);
	blockData[0].vCenterStretching = { 0.0, 0.66, 0 };
	blockData[0].vCenterPosition = { 0.0, 0.0, 0.0 };

	for (int i = 0; i < 3; curIndex[i] = i, i++);
	blockData[0].nIndexCount = 3;
	blockData[0].nVertexCount = 3;

	//Block 1 - 
}

static void GetVertexData(const Block& block, RendererVertex outVertex[])
{
	BlockAdditionalData& data = blockData[block.blockId];
	glm::mat4 mat;
	glm::mat4 matUnity = glm::mat4(1.0f);
	mat = glm::scale(matUnity, block.scale);

	glm::vec3 dirToPos = data.vCenterStretching - data.vCenterPosition;
	glm::vec4 centerPos = { dirToPos.x ,dirToPos.y, dirToPos.z, 1.0 };
	centerPos = mat * centerPos;
	dirToPos = { centerPos.x, centerPos.y, centerPos.z };
	//dirToPos /= centerPos.w;	//Keep or delete ?

	//first move the object to the origin, scale it, then move it to its current position
	//the position vector describes the coordinates of the vCenterPosition point
	
	mat = glm::translate(matUnity, dirToPos + block.position) *
		  glm::scale(matUnity, block.scale) *
		  glm::translate(matUnity, -data.vCenterStretching);

	memcpy(outVertex, data.vertex, data.nVertexCount * sizeof(RendererVertex));

	for (unsigned int i = 0; i < data.nVertexCount; i++)
	{
		glm::vec3& vertexPos = outVertex[i].m_pos;
		glm::vec4 pos = { vertexPos.x, vertexPos.y, vertexPos.z, 1.0f };
		pos = mat * pos;
		vertexPos = { pos.x, pos.y, pos.z };
	}
}
BlockSpawnerLayer::BlockSpawnerLayer() :
	m_pPlayerLayer (nullptr)
{
	m_blocks.Reserve(5);

	CreateShapes();

	BlockSpawnerLayer::ResetLayer();
}
void BlockSpawnerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
	pApp->RegisterEvents(LayerMouseMove, nIndex);
}
void BlockSpawnerLayer::ResetLayer()
{
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
	block.blockId = 0;
	block.position = { 400, 400, 0 };
	block.scale = { 40, 40, 1 };
	block.velocity = { -0, 0, 0 };

}
void BlockSpawnerLayer::OnUpdate(float deltaTime)
{
	Block* buffer = m_blocks.Buffer();
	int index = m_blocks.Begin();
	
	//Check if the first block in the queue is out of bounds. Blocks move at the same speed so the first one in the queue is the only one that needs to be checked instead of every single block
	if (m_blocks.Count())
	{
		//Block has reached passed the screen
		if (buffer[0].position.x + buffer[0].scale.x < 0)
		{
			m_blocks.Pop();
		}
	}
	
	bool bTemp = false;

	const RendererVertex* playerVertex = m_pPlayerLayer->GetVertex();
	unsigned int nPlayerVertexCount = m_pPlayerLayer->GetVertexCount();
	for (unsigned int i = 0; i < m_blocks.Count(); i++, index++, index %= m_blocks.Size())
	{
		//Move the block
		Block& curBlock = buffer[index];
		curBlock.position += curBlock.velocity * deltaTime;


		//Render the block
		BlockAdditionalData& data = blockData[curBlock.blockId];
		RendererVertex blockVertex[6];
		GetVertexData(curBlock, blockVertex);

		if (Collision::CheckCollision(playerVertex, nPlayerVertexCount, blockVertex, data.nVertexCount))
		{
			bTemp = true;
		}

		Renderer::DrawQuadColor(blockVertex, data.nVertexCount, data.index, data.nIndexCount);
	}

	glm::vec4 col = (bTemp) ? glm::vec4{1.0, 0.0, 0.0, 1.0} : glm::vec4{0.7, 0.7, 0.7, 1.0};
	for (int i = 0; i < 3; i++)
		blockData[0].vertex[i].m_col = col;

}
bool BlockSpawnerLayer::OnWindowResize(int width, int height)
{
	return false;
}
bool BlockSpawnerLayer::OnMouseMove(double x, double y)
{
	if (m_blocks.Count())
	{
		Block* block = m_blocks.GetAt(0);
		//LOG_INFO("x {0}, y{1}", x, y);
		block->position = { x, y, 1 };
	}

	return false;
}
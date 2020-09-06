#pragma once

#include "Layer.h"
#include "Core/CircularQueue.h"
#include "Maths.h"
#include "Renderer/RendererShapes.h"

class AbstractPlayerLayer;

struct Block
{
	Block() = default;
	Block(const Block&) = default;
	
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec4 color;

	glm::vec3 velocity;
	RendererShapes::Shape shape;

	glm::vec2 phaseRange;
	bool isPhasable;
};

class BlockSpawnerLayer : public Layer
{
public:
	BlockSpawnerLayer();
	virtual void RegisterEvents(Application* pApp, int nIndex) override;

	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;	//in seconds
	
	virtual bool OnMouseMove(int x, int y) override;

	inline unsigned int GetBlockCount() const { return m_blocks.Count(); }

	void RecalculateBlockPhase(float mousePosX);

protected:
	typedef void (*CreateBlockFunc) (CircularQueue<Block>& blocks, double& nextSpawnTime, double curPhase);

	//This is mainly for the tutorial layer only because this layer moves and renders it in the same for loop instead of 2 for loops (WOuldnt make much much of a difference because 
	void RenderBlocks();	
	inline CircularQueue<Block>& GetBlocks() { return m_blocks; }

	inline void SetNextSpawnTime(double time) { m_dNextSpawnTime = time; }

	CreateBlockFunc m_CreateBlockFunc;

private:
	inline void ClearOutOfBoundsBlocks();
	void MoveCollisionRenderBlocks(float deltaTime);

private:
	CircularQueue<Block> m_blocks;
	AbstractPlayerLayer* m_pPlayerLayer;

	double m_dNextSpawnTime;
	bool m_bPreviousCollided;
	double m_dCurPhasePercent;	//x percent

	unsigned int m_nBlockTextureId;
};
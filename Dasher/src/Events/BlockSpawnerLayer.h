#pragma once

#include "Layer.h"
#include "Core/CircularQueue.h"
#include "Maths.h"
#include "Renderer/RendererShapes.h"

class RendererVertex;
class PlayerLayer;

struct Block
{
	Block() = default;
	Block(Block&) = default;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec4 color;

	glm::vec3 velocity;
	RendererShapes::Shape shape;

	double phasePercent;
	bool isPhasable;
};

class BlockSpawnerLayer : public Layer
{
public:
	BlockSpawnerLayer();
	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual void ResetLayer() override;

	virtual void OnStart();
	virtual void OnUpdate(float deltaTime);	//in seconds
	
#if 0
	virtual bool OnWindowResize(int width, int height) override;
	virtual bool OnMouseMove(int x, int y) override;
#endif

private:
	inline void ClearOutOfBoundsBlocks();
	void CreateBlock();
private:
	CircularQueue<Block> m_blocks;
	PlayerLayer* m_pPlayerLayer;

	//spawn blocks
	const double m_dTimeBwSpawnMin;
	const double m_dTimeBwSpawnMax;

	//size
	const double m_dSizeXMin;
	const double m_dSizeXMax;
	const double m_dSizeYMin;
	const double m_dSizeYMax;
	

	double m_dNextSpawnTime;
};
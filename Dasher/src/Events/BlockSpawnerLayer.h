#pragma once

#include "Layer.h"
#include "Core/CircularQueue.h"
#include "Maths.h"

class RendererVertex;
class PlayerLayer;
struct Block
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 velocity;
	unsigned char blockId;	//used as an index for the vertexBuffer
};
class BlockSpawnerLayer : public Layer
{
public:
	BlockSpawnerLayer();
	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual void ResetLayer() override;

	virtual void OnStart();
	virtual void OnUpdate(float deltaTime);	//in seconds
	virtual bool OnWindowResize(int width, int height) override;
	virtual bool OnMouseMove(double x, double y) override;


private:
	CircularQueue<Block> m_blocks;
	PlayerLayer* m_pPlayerLayer;
};
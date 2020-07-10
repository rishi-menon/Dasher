#pragma once

#include "Layer.h"

class BlockSpawnerLayer : public Layer
{
public:
	BlockSpawnerLayer();
	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual void ResetLayer() override;

	virtual void OnStart();
	virtual void OnUpdate(float deltaTime);	//in seconds
	virtual bool OnWindowResize(int width, int height) override;

private:


};
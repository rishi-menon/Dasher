#pragma once

#include "Layer.h"
#include "Maths.h"
#include "Renderer/RendererVertex.h"

class BackgroundLayer : public Layer
{
public:
	BackgroundLayer();
	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual void ResetLayer() override;

	virtual void OnStart();
	virtual void OnUpdate(float deltaTime);	//in seconds
	virtual bool OnWindowResize(int width, int height) override;

private:

	unsigned int m_nTextureId;
	RendererVertex m_vertex[4];
	const int m_nIndexCount = 6;
	const unsigned int m_Index[6] = { 0,1,2,2,3,0 };

	const float m_nVelocityX = -0.06;
};
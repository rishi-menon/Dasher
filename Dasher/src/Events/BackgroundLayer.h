#pragma once

#include "Layer.h"
#include "Maths.h"
#include "Renderer/RendererVertex.h"
#include "AssetManagement/StandardTexture.h"

struct BackgroundLayerProps
{
	BackgroundLayerProps();
	BackgroundLayerProps(StandardTexture type, float velocity = -0.012f);
	BackgroundLayerProps(const BackgroundLayerProps&) = default;
	void SetInitTexCoords(const glm::vec2& bottomLeft, const glm::vec2& topRight);

	float velocityX;
	glm::vec2 initTexCoords[4];
	StandardTexture textureType;

};
class BackgroundLayer : public Layer
{
public:
	BackgroundLayer(const BackgroundLayerProps& props);
	virtual ~BackgroundLayer();
		
	virtual void RegisterEvents(Application* pApp, int nIndex) override;

	virtual void OnStart();
	virtual void OnUpdate(float deltaTime);	//in seconds
	virtual bool OnWindowResize(int width, int height) override;

	BackgroundLayerProps GetBackgroundProp() const;

private:
	unsigned int m_nTextureId;
	RendererVertex m_vertex[4];
	float m_nVelocityX;
};
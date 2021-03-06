
#include "BackgroundLayer.h"
#include "Application/Application.h"
#include "AssetManagement/Texture.h"
#include "Renderer/Renderer.h"

BackgroundLayerProps::BackgroundLayerProps() :
	velocityX (-0.012f),
	textureType(StandardTexture::None)
{
	SetInitTexCoords(glm::vec2(0.0f, 0.0f), glm::vec2(0.75f, 1.0f));
}
BackgroundLayerProps::BackgroundLayerProps(StandardTexture type, float velocity /*= -0.012f*/) :
	velocityX(velocity),
	textureType(type)
{
	SetInitTexCoords(glm::vec2(0.0f, 0.0f), glm::vec2(0.75f, 1.0f));
}
void BackgroundLayerProps::SetInitTexCoords(const glm::vec2& bottomLeft, const glm::vec2& topRight)
{
	initTexCoords[0] = { bottomLeft.x, bottomLeft.y };
	initTexCoords[1] = { topRight.x, bottomLeft.y };
	initTexCoords[2] = { topRight.x, topRight.y };
	initTexCoords[3] = { bottomLeft.x, topRight.y };
}
////////////////////////////////////////////////////////////////////////////////////////////////
BackgroundLayer::BackgroundLayer(const BackgroundLayerProps& props) :
	m_fVelocityX(0.0),
	textureType (StandardTexture::None)
{
#if 0
	ASSERT(props.textureType != StandardTexture::None, "Standard texture was null");

	m_nTextureId = GetStandardTextureId(props.textureType);

	const glm::vec4 col = { 1.0,1.0,1.0,1.0 };

	int width = Application::GetWidth();
	int height = Application::GetHeight();

	m_vertex[0].SetPosColTex({ 0.0f, 0.0f, -0.9 }, col, props.initTexCoords[0]);
	m_vertex[1].SetPosColTex({ width, 0.0f, -0.9 }, col, props.initTexCoords[1]);
	m_vertex[2].SetPosColTex({ width, height, -0.9 }, col, props.initTexCoords[2]);
	m_vertex[3].SetPosColTex({ 0.0f, height, -0.9 }, col, props.initTexCoords[3]);
#else	
	SetBackgroundState(props);
#endif
}

void BackgroundLayer::SetBackgroundState(const BackgroundLayerProps& props)
{
	textureType = props.textureType;

	ASSERT(textureType != StandardTexture::None, "Standard texture was null");
	if (textureType != StandardTexture::None)
	{
		m_fVelocityX = props.velocityX;
		m_nTextureId = GetStandardTextureId(props.textureType);

		const glm::vec4 col = { 1.0,1.0,1.0,1.0 };
		int width = Application::GetWidth();
		int height = Application::GetHeight();

		m_vertex[0].SetPosColTex({ 0.0f, 0.0f, -0.9 }, col, props.initTexCoords[0]);
		m_vertex[1].SetPosColTex({ width, 0.0f, -0.9 }, col, props.initTexCoords[1]);
		m_vertex[2].SetPosColTex({ width, height, -0.9 }, col, props.initTexCoords[2]);
		m_vertex[3].SetPosColTex({ 0.0f, height, -0.9 }, col, props.initTexCoords[3]);
	}
}

BackgroundLayer::~BackgroundLayer()
{
}
void BackgroundLayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
}

void BackgroundLayer::OnStart()
{
}
void BackgroundLayer::OnUpdate(float deltaTime)
{
	if (textureType == StandardTexture::None) { return; }

	for (int i = 0; i < 4; i++)
	{
		//subtract the value because when the speed is positive, the texture coordinates should be decreasing to give the effect of the background moving towards the right
		m_vertex[i].m_textureCoord.x -= m_fVelocityX * deltaTime;
	}
	
	float value = m_vertex[0].m_textureCoord.x;
	//Shift the coordinates to the left or right to prevent them from getting too large or small and overflowing
	if (value < -2)
	{
		for (int i = 0; i < 4; m_vertex[i].m_textureCoord.x += 1, i++);
	} 
	else if (value > 2)
	{
		for (int i = 0; i < 4; m_vertex[i].m_textureCoord.x -= 1, i++);
	}

	Renderer::DrawQuadTexture(m_vertex, RendererShapes::Shape::ShapeQuad, m_nTextureId);
}
bool BackgroundLayer::OnWindowResize(int width, int height)
{
	m_vertex[0].m_pos = { 0.0f, 0.0f, 0.0f };
	m_vertex[1].m_pos = { width, 0.0f, 0.0f };
	m_vertex[2].m_pos = { width, height, 0.0f };
	m_vertex[3].m_pos = { 0.0f, height, 0.0f };
	return false;
}

BackgroundLayerProps BackgroundLayer::GetBackgroundProp() const
{
	BackgroundLayerProps props(textureType, m_fVelocityX);
	props.SetInitTexCoords(m_vertex[0].m_textureCoord, m_vertex[2].m_textureCoord);
	return props;
}
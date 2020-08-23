
#include "BackgroundLayer.h"
#include "Application/Application.h"
#include "AssetManagement/Texture.h"
#include "Renderer/Renderer.h"

BackgroundLayerProps::BackgroundLayerProps() :
	velocityX (-0.012f),
	path (nullptr)
{
	SetInitTexCoords(glm::vec2(0.0f, 0.0f), glm::vec2(0.75f, 1.0f));
}
BackgroundLayerProps::BackgroundLayerProps(const char* const path, float velocity /*= -0.012f*/) :
	velocityX(velocity),
	path (path)
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
	m_nVelocityX (props.velocityX)
{
	ASSERT(props.path, "Image path was null");
	m_nTextureId = Texture::LoadTexture(props.path, nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));

	const glm::vec4 col = { 1.0,1.0,1.0,1.0 };

	int width = Application::GetWidth();
	int height = Application::GetHeight();

	m_vertex[0].SetPosColTex({ 0.0f, 0.0f, -0.5f }, col, props.initTexCoords[0]);
	m_vertex[1].SetPosColTex({ width, 0.0f,-0.5f }, col, props.initTexCoords[1]);
	m_vertex[2].SetPosColTex({ width, height, -0.5f }, col, props.initTexCoords[2]);
	m_vertex[3].SetPosColTex({ 0.0f, height, -0.5f }, col, props.initTexCoords[3]);
}

BackgroundLayer::~BackgroundLayer()
{
	Texture::DeleteTexture(m_nTextureId);
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
	for (int i = 0; i < 4; i++)
	{
		//subtract the value because when the speed is positive, the texture coordinates should be decreasing to give the effect of the background moving towards the right
		m_vertex[i].m_textureCoord.x -= m_nVelocityX * deltaTime;
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
	BackgroundLayerProps props("", m_nVelocityX);
	props.SetInitTexCoords(m_vertex[0].m_textureCoord, m_vertex[2].m_textureCoord);
	return props;
}

#include "BackgroundLayer.h"
#include "Application/Application.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

BackgroundLayer::BackgroundLayer()
{
	//m_nTextureId = Texture::LoadTexture("Assets\\Textures\\Space.jpg", nullptr, nullptr, TextureProperties(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT));
	m_nTextureId = Texture::LoadTexture("Assets\\Textures\\External\\bg3.png", nullptr, nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));

	ResetLayer();
}
void BackgroundLayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
}
void BackgroundLayer::ResetLayer()
{
	const glm::vec4 col = { 1.0,1.0,1.0,1.0 };
	const glm::vec2 texCordBottomLeft = { 0.0f, 0.0f };
	const glm::vec2 texCordBottomRight = { 0.75f, 0.0f };
	const glm::vec2 texCordTopRight = { 0.75f, 1.0f };
	const glm::vec2 texCordTopLeft = { 0.0f, 1.0f };

	int width  = Application::GetCurrentApp()->GetWidth();
	int height = Application::GetCurrentApp()->GetHeight();

	m_vertex[0].SetPosColTex({ 0.0f, 0.0f, -0.5f },	  col, texCordBottomLeft);
	m_vertex[1].SetPosColTex({ width, 0.0f,-0.5f },	  col, texCordBottomRight);
	m_vertex[2].SetPosColTex({ width, height, -0.5f }, col, texCordTopRight);
	m_vertex[3].SetPosColTex({ 0.0f, height, -0.5f },  col, texCordTopLeft);
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
	
	//float value = m_vertex[0].m_textureCoord.x;
	//if (value < -2)
	//{
	//	//shift coords to the right by 1
	//	for (int i = 0; i < 4; i++)
	//	{
	//		m_vertex[i].m_textureCoord.x += 1;
	//	}
	//} 
	//else if (value > 1)
	//{
	//	for (int i = 0; i < 4; i++)
	//	{
	//		m_vertex[i].m_textureCoord.x -= 1;
	//	}
	//}

	Renderer::DrawQuadTexture(m_vertex, 6, m_Index, m_nIndexCount, m_nTextureId);
}
bool BackgroundLayer::OnWindowResize(int width, int height)
{
	m_vertex[0].m_pos = { 0.0f, 0.0f, 0.0f };
	m_vertex[1].m_pos = { width, 0.0f, 0.0f };
	m_vertex[2].m_pos = { width, height, 0.0f };
	m_vertex[3].m_pos = { 0.0f, height, 0.0f };
	return false;
}
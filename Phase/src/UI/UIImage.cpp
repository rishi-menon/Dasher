
#include "UIImage.h"
#include "Log.h"
#include "Renderer/Renderer.h"

UIImage::UIImage() :
	m_nTextureId ((unsigned int)(-1)),
	m_curPos (0, 0 ,0)
{
}

void UIImage::OnUpdate(float deltaTime)
{
	if (m_nTextureId != (unsigned int)(-1))
	{
		Renderer::DrawQuadTexture(m_vertex, RendererShapes::Shape::ShapeQuad, m_nTextureId);
	}
}

bool UIImage::OnWindowResize(int x, int y)
{
	return false;
}

void UIImage::SetProperties(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& col)
{
	m_curPos = pos;
	RendererShapes::Rectangle(m_vertex, pos, size, col);
}

void UIImage::SetPos(const glm::vec3& pos)
{
	glm::vec3 delta = pos - m_curPos;
	for (int i = 0; i < 4; i++)
	{
		m_vertex[i].m_pos += delta;
	}
	m_curPos = pos;
}

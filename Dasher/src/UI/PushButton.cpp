#include "PushButton.h"
#include "Renderer/Renderer.h"
#include "Application/Application.h"

PushButtonProps::PushButtonProps(UITypes uitype, const glm::ivec2& buttonSize, const glm::vec4& col) :
	type(uitype),
	size (buttonSize),
	color (col)
{
}
void PushButtonProps::SetBasicProps(UITypes uitype, const glm::ivec2& buttonSize, const glm::vec4& col)
{
	type = uitype;
	size = buttonSize;
	color = col;
}

////////////////////////////////////////////////////////////

PushButton::PushButton() :
	m_bIsEnabled (false),
	m_bOldEnabled (true),
	m_vPos (0.0f, 0.0f),
	m_toggleFunc(nullptr)
{
}

void PushButton::OnUpdate(float deltaTime)
{
	const PushButtonProps& prop = m_props[m_bIsEnabled];

	if (m_bOldEnabled != m_bIsEnabled)
	{
		m_bOldEnabled = m_bIsEnabled;
		RegenerateVertexBuffer();
	}

	Renderer::DrawQuadTexture(m_vertex, RendererShapes::Shape::ShapeQuad, UI::GetTextureId(prop.type));
}
void PushButton::RegenerateVertexBuffer()
{
	const PushButtonProps& prop = m_props[m_bIsEnabled];
	RendererShapes::Rectangle(m_vertex, m_vPos, prop.size, prop.color);
}

bool PushButton::OnMouseUp(int nButton)
{
	const TextureDimensions& curDim = UI::GetTextureDimension(m_props[m_bIsEnabled].type);
	double x, y;
	Application::GetCurrentApp()->GetMousePos(x, y);

	if (UI::CheckCollision((int)x, (int)y, curDim, m_vertex))
	{
		m_bIsEnabled = !m_bIsEnabled;
		if (m_toggleFunc)
		{
			m_toggleFunc(m_bIsEnabled);
		}
		return true;
	}
	return false;
}

bool PushButton::OnWindowResize(int x, int y)
{
	return false;
}
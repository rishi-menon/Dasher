#include "Button.h"
#include "Log.h"
#include "Renderer/Renderer.h"
#include "Application/Application.h"


void ButtonProps::SetBasicProps(UITypes uitype, const glm::ivec2& buttonSize, const glm::vec4& col)
{
	type = uitype;
	size = buttonSize;
	color = col;
}
void ButtonProps::SetTextProps(std::string strText, const glm::vec2& textOffset, float textScale, const glm::vec4& textCol)
{
	text = std::move (strText);
	textPosOffset = textOffset;
	this->textScale = textScale;
	textColor = textCol;
}


//////////////////////////////////////////////////////////////////////
Button::Button() :
	m_curState(StateDefault),
	m_lastState(StateDefault),
	m_vPos(0.0f, 0.0f, 0.8f),
	m_clickFunc(nullptr),
	m_userData (nullptr),
	m_nOptionalKey(-1)
{

}

void Button::RegenerateVertexBuffer()
{
	//if (m_curState != StateNone)
	{
		const ButtonProps& prop = m_buttonProps[m_curState];
		RendererShapes::Rectangle(m_vertex, m_vPos, prop.size, prop.color);
	}
}
void Button::OnUpdate(float deltaTime)
{
	//Render here
	if (m_curState != m_lastState)
	{
		m_lastState = m_curState;
		RegenerateVertexBuffer();
	}
	//if (m_curState != StateNone)
	{
		const ButtonProps& prop = m_buttonProps[m_curState];
		Renderer::DrawQuadTexture(m_vertex, RendererShapes::Shape::ShapeQuad, UI::GetTextureId(prop.type));
		
		//middle of the button on y axis and from the leftmost point of the button (this is without any offset)
		float posX = m_vPos.x - prop.size.x * 0.5f + prop.textPosOffset.x;
		float posY = m_vPos.y + prop.textPosOffset.y;

		Renderer::DrawTextColor(prop.text.c_str(), prop.text.size(), posX, posY, prop.textScale, prop.textColor);
	}
}

bool Button::OnMouseMove(int x, int y)
{
	const TextureDimensions& curDim = UI::GetTextureDimension(m_buttonProps[m_curState].type);
	if (UI::CheckCollision(x, y, curDim, m_vertex))
	{
		//It could be in button press mode in which case dont do anything
		if (m_curState == StateDefault)
		{
			m_curState = StateSelected;
		}
		return true;
	}
	else
	{
		m_curState = StateDefault;
		return false;
	}
}
bool Button::OnMouseDown(int nButton)
{
	if (!nButton && m_curState == StateSelected)
	{
		m_curState = StateClicked;
		return true;
	}
	return false;

}
bool Button::OnMouseUp(int nButton)
{
	if (nButton) { return false; }	//it wasnt a left click

	if (m_curState == StateClicked)
	{
		m_curState = StateSelected;
		if (m_clickFunc)
		{
			m_clickFunc(m_userData);
		}
		return true;
	}
	else
	{
		double x, y;
		Application::GetCurrentApp()->GetMousePos(x, y);
		
		const TextureDimensions& curDim = UI::GetTextureDimension(m_buttonProps[m_curState].type);

		if (UI::CheckCollision(static_cast<int>(x), static_cast<int>(y), curDim, m_vertex))
		{
			m_curState = StateSelected;
		}
		else
		{
			m_curState = StateDefault;
		}
		return false;
	}
}
	
bool Button::OnWindowResize(int x, int y)
{
	//LOG_INFO("window resize: {0}, {1}", x, y);
	return false;
}

void Button::ManualClick()
{
	double x, y;
	Application::GetCurrentApp()->GetMousePos(x, y);

	const TextureDimensions& curDim = UI::GetTextureDimension(m_buttonProps[m_curState].type);

	if (UI::CheckCollision(static_cast<int>(x), static_cast<int>(y), curDim, m_vertex))
	{
		m_curState = StateSelected;
	}
	else
	{
		m_curState = StateDefault;
	}

	if (m_clickFunc)
	{
		m_clickFunc(m_userData);
	}
}

bool Button::OnKeyUp(int key)
{
	if (m_nOptionalKey != -1 && m_nOptionalKey == key)
	{
		ManualClick();
	}
	return false;
}
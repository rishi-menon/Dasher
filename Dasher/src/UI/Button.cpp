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
	m_vPos(0.0f, 0.0f),
	m_clickFunc(nullptr)
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

		Renderer::DrawTextColor(prop.text, posX, posY, prop.textScale, prop.textColor);		
	}
}

bool Button::OnMouseMove(int x, int y)
{
	if (CheckCollision(x, y))
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
			m_clickFunc();
		}
		return true;
	}
	else
	{
		double x, y;
		Application::GetCurrentApp()->GetMousePos(x, y);

		if (CheckCollision(static_cast<int>(x), static_cast<int>(y)))
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

bool Button::CheckCollision(int mousex, int mousey)
{
	const TextureDimensions& curDim = UI::GetTextureDimension(m_buttonProps[m_curState].type);

	double percentX = Math::GetPercent(m_vertex[0].m_pos.x, m_vertex[1].m_pos.x, mousex);
	double percentY = Math::GetPercent(m_vertex[0].m_pos.y, m_vertex[2].m_pos.y, mousey);

	//LOG_WARN("PX: {0}, PY:{1}", percentX, percentY);
	if (percentX < 0 || percentX > 1 || percentY < 0 || percentY > 1)
	{
		m_curState = StateDefault;
		return false;
	}

	int textureCoordX = static_cast<int>(percentX * curDim.width);
	int textureCoordY = static_cast<int>(percentY * curDim.height);

	int index = textureCoordY * curDim.width + textureCoordX;

	if (curDim.bpp == 4)
	{
		//check alpha value 
		return (curDim.buffer[curDim.bpp * index + 3] > 0);
	}
	else
	{
		//check the color channel ie check if its black or not
		int rawIndex = curDim.bpp * index;
		for (int i = 0; i < curDim.bpp; i++)
		{
			if (curDim.buffer[rawIndex + i] > 0) { return true; }
		}
		return false;
	}
}
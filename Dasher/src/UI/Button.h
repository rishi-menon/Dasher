#pragma once
#include "Maths.h"
#include "UIObject.h"

struct ButtonProps
{
	unsigned int texId;
	glm::ivec2 size;
	glm::vec4 color;
};

class Button : public UIObject
{
public:
	enum ButtonState : unsigned int
	{
		StateDefault = 0,
		StateSelected,
		StateClicked,

		Count
	};

public:
	Button() = default;
	Button(const Button&) = default;

	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDown(int nButton)  override;
	virtual bool OnMouseUp(int nButton)    override;

	virtual bool OnKey(int key)		override;
	virtual bool OnKeyDown(int key) override;
	virtual bool OnKeyUp(int key)	override;

	virtual bool OnWindowResize(int x, int y) override;

private:
	ButtonProps m_buttonProps[Count];
	ButtonState m_curState;
};
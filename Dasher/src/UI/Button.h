#pragma once
#include "Maths.h"
#include "UIObject.h"
#include "Log.h"
#include "Renderer/RendererVertex.h"
#include "UI/UIObject.h"

struct ButtonProps
{
	UITypes type;
	glm::ivec2 size;
	glm::vec4 color;
};

class Button : public UIObject
{
public:
	enum ButtonState : int
	{
		//StateNone = -1,
		StateDefault  = 0,
		StateSelected,
		StateClicked, 

		StateCount
	};

public:
	Button();
	Button(const Button&) = default;

	//Events
	virtual void OnUpdate(float deltaTime) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDown(int nButton)  override;
	virtual bool OnMouseUp(int nButton)    override;
	//virtual bool OnKey(int key)		override;
	//virtual bool OnKeyDown(int key) override;
	//virtual bool OnKeyUp(int key)	override;
	virtual bool OnWindowResize(int x, int y) override;
	

	inline void SetStateProperties(const ButtonProps& prop, int index) { ASSERT(index < StateCount, "Index out of range"); m_buttonProps[index] = prop; }

	inline void SetCurrentState(ButtonState state) { m_curState = state; }

	void SetPosition(const glm::vec2& pos) { m_vPos = pos; RegenerateVertexBuffer(); }

	void RegenerateVertexBuffer();

private:
	bool CheckCollision(int mousex, int mousey);


private:
	ButtonProps m_buttonProps[StateCount];
	ButtonState m_curState, m_lastState;

	glm::vec2 m_vPos;
	RendererVertex m_vertex[4];

};
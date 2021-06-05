#pragma once
#include "Maths.h"
#include "UIObject.h"
#include "Log.h"
#include "Renderer/RendererVertex.h"
#include "UI/UIObject.h"
#include <string>

struct ButtonProps
{

	ButtonProps() = default;
	ButtonProps(const ButtonProps&) = default;

	void SetBasicProps(UITypes uitype, const glm::ivec2& buttonSize, const glm::vec4& col);
	void SetTextProps(std::string strText, const glm::vec2& textOffset, float textScale, const glm::vec4& textCol);

	UITypes type;
	glm::ivec2 size;
	glm::vec4 color;

	std::string text;
	glm::vec2 textPosOffset;
	float textScale;
	glm::vec4 textColor;
};

class Button : public UIObject
{
	typedef void (*ButtonClickFunc) (void*);

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
	virtual ~Button() {}

	//Events
	virtual void OnUpdate(float deltaTime) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDown(int nButton)  override;
	virtual bool OnMouseUp(int nButton)    override;
	
	virtual bool OnKeyUp(int key)	override;
	virtual bool OnWindowResize(int x, int y) override;
	

	inline void SetStateProperties(const ButtonProps& prop, int index) { ASSERT(index < StateCount, "Index out of range"); m_buttonProps[index] = prop; }

	inline void SetCurrentState(ButtonState state) { m_curState = state; }
	inline void SetPosition(const glm::vec3& pos) { m_vPos = pos; RegenerateVertexBuffer(); }
	inline void SetButttonClickEvent(ButtonClickFunc func) { m_clickFunc = func; }
	inline void SetUserData(void* data) { m_userData = data; }
	inline void SetOptionalKey(int nKey) { m_nOptionalKey = nKey; }

	void RegenerateVertexBuffer();
	
	void ManualClick();
private:
	ButtonProps m_buttonProps[StateCount];
	ButtonState m_curState, m_lastState;

	glm::vec3 m_vPos;
	RendererVertex m_vertex[4];

	void* m_userData;	//Can be any data... It gets passed to the event callbackfunction
	ButtonClickFunc m_clickFunc;

	int m_nOptionalKey;	//When this key is pressed then it simulates a button click
};
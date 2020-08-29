#pragma once

#include "UIObject.h"

struct PushButtonProps
{
	PushButtonProps() = default;
	PushButtonProps(const PushButtonProps&) = default;

	PushButtonProps(UITypes uitype, const glm::ivec2& buttonSize, const glm::vec4& col);

	void SetBasicProps(UITypes uitype, const glm::ivec2& buttonSize, const glm::vec4& col);

	UITypes type;
	glm::ivec2 size;
	glm::vec4 color;
};

class PushButton : public UIObject
{
	typedef void (*PushButtonToggleFunc) (bool);
public:
	PushButton();

	virtual void OnUpdate(float deltaTime) override;
	virtual bool OnMouseUp(int nButton)    override;
	virtual bool OnWindowResize(int x, int y) override;

	inline void SetStateProperties(bool stateEnabled, const PushButtonProps& prop) { m_props[stateEnabled] = prop; }

	inline void SetEnabled(bool bIsEnabled) { m_bIsEnabled = bIsEnabled; }
	inline bool GetEnabled() const { return m_bIsEnabled; }
	inline bool& GetEnabledRef() { return m_bIsEnabled; }

	inline void SetPos(const glm::vec3& pos) { m_vPos = pos; RegenerateVertexBuffer(); }
	void RegenerateVertexBuffer();

	inline void SetButttonClickEvent(PushButtonToggleFunc func) { m_toggleFunc = func; }

private:
	bool m_bIsEnabled;
	bool m_bOldEnabled;

	PushButtonProps m_props[2];
	glm::vec3 m_vPos;

	RendererVertex m_vertex[4];

	PushButtonToggleFunc m_toggleFunc;	//optional
};
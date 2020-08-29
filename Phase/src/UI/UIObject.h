#pragma once
#include "AssetManagement/Texture.h"
#include "Renderer/RendererVertex.h"

enum class UITypes : unsigned int
{
	Button0 = 0,

	ButtonBack,
	ButtonBack_S,	//Selected state

	ButtonRestart,
	ButtonRestart_S,	//Selected state

	PushButton0,
	PushButton0_E,	//Enabled state	

	Count
};

namespace UI
{
	extern unsigned int UITexIds[(unsigned int)UITypes::Count];
	extern TextureDimensions UITexDimension[(unsigned int)UITypes::Count];

	inline unsigned int GetTextureId(UITypes type) { return UI::UITexIds[static_cast<unsigned int>(type)]; }
	inline const TextureDimensions& GetTextureDimension (UITypes type) { return UI::UITexDimension[static_cast<unsigned int>(type)]; }

	bool UIInit();
	void UICleanup();
	bool CheckCollision(int mousex, int mousey, const TextureDimensions& curDim, const RendererVertex vertexQuad[4]);

}

class UILayer;
class UIObject
{
public:
	UIObject();
	virtual ~UIObject();
	
	virtual void OnUpdate(float deltaTime) {}

	virtual bool OnMouseMove(int x, int y)		{ return false; }
	virtual bool OnMouseDown(int nButton)		{ return false; }
	virtual bool OnMouseUp(int nButton)			{ return false; }

	virtual bool OnKey(int key)					{ return false; }
	virtual bool OnKeyDown(int key)				{ return false; }
	virtual bool OnKeyUp(int key)				{ return false; }

	virtual bool OnWindowResize(int x, int y)	{ return false; }
	
	inline bool GetIsActive() const { return m_bIsActive; }

	inline void SetIsActive(bool bIsActive) { m_bIsActive = bIsActive; }

private:
	void RegisterUIObject();	//Registers this object to the layer

private:
	//Instead of de-registering the object from the Layer which involves removing the element from the middle of the array, the isActive would control when the object receives events and when it doesnt
	bool m_bIsActive;
};
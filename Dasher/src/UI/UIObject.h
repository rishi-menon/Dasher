#pragma once

enum class UITypes : unsigned int
{
	Button0 = 0,

	Count
};

namespace UI
{
	extern unsigned int UITexIds[(unsigned int)UITypes::Count];
	
	void UIInit();
	void UICleanup();

	inline unsigned int GetTextureId(UITypes type) { return UI::UITexIds[static_cast<unsigned int>(type)]; }
	
}

class UILayer;
class UIObject
{
public:
	UIObject();
	virtual ~UIObject() {}
	
	virtual bool OnMouseMove(int x, int y)		{ return false; }
	virtual bool OnMouseDown(int nButton)		{ return false; }
	virtual bool OnMouseUp(int nButton)			{ return false; }

	virtual bool OnKey(int key)					{ return false; }
	virtual bool OnKeyDown(int key)				{ return false; }
	virtual bool OnKeyUp(int key)				{ return false; }

	virtual bool OnWindowResize(int x, int y)	{ return false; }
	
	inline bool GetIsActive() const { return m_bIsActive; }

private:
	void RegisterUIObject();	//Registers this object to the layer

private:
	//Instead of de-registering the object from the Layer which involves removing the element from the middle of the array, the isActive would control when the object receives events and when it doesnt
	bool m_bIsActive;
};
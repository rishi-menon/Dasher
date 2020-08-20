#pragma once
#include "Layer.h"
#include "UI/UIObject.h"
#include <vector>

class UILayer;
extern UILayer* g_pCurrentUILayer;

class UILayer : public Layer
{
public:
	UILayer();
	virtual ~UILayer();

	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual void OnStart();

	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDown(int nButton)  override;
	virtual bool OnMouseUp(int nButton)	   override;

	virtual bool OnKey(int key)     override;
	virtual bool OnKeyDown(int key) override;
	virtual bool OnKeyUp(int key)   override;

	virtual bool OnWindowResize(int x, int y) override;

	inline void RegisterUIObject(UIObject* pObj) { m_vObjects.emplace_back(pObj); }
private:
	std::vector<UIObject*> m_vObjects;
};
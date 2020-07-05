#pragma once

#include "Layer.h"

class PlayerLayer : public Layer
{
	virtual void RegisterEvents(Application* pApp, int nIndex) override;

	virtual void OnStart();
	virtual void OnUpdate(float deltaTime);	//in seconds

	virtual bool OnMouseMove(double x, double y);
	virtual bool OnMouseDown(int nButton);
	virtual bool OnMouseUp(int nButton);

	virtual bool OnKey(int key);
	virtual bool OnKeyDown(int key);
	virtual bool OnKeyUp(int key);

	virtual bool OnWindowResize(int x, int y) override;

};
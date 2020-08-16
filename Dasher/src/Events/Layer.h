#pragma once

using LayerIndex = unsigned short;

enum Layers : unsigned short
{
	LayerMouseMove = 0x0000,
	LayerMouseDown,
	LayerMouseUp,

	LayerKey,
	LayerKeyDown,
	LayerKeyUp,

	LayerWindowResize,

	LayerCount
};

class Application;

class Layer
{
public:
	virtual ~Layer() {}

	virtual void RegisterEvents(Application* pApp, int nIndex) {}
	virtual void ResetLayer() {}

	virtual void OnStart() {}
	virtual void OnUpdate(float deltaTime) {}	//in seconds

	virtual bool OnMouseMove(int x, int y) { return false; }
	virtual bool OnMouseDown(int nButton) { return false; }
	virtual bool OnMouseUp(int nButton) { return false; }

	virtual bool OnKey(int key) { return false; }
	virtual bool OnKeyDown(int key) { return false; }
	virtual bool OnKeyUp(int key) { return false; }

	virtual bool OnWindowResize(int x, int y) { return false; }

};
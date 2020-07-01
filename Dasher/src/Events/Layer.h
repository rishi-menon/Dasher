#pragma once

class Layer
{
public:

	virtual void OnStart() {}
	virtual void OnUpdate(float deltaTime) {}	//in seconds

	virtual bool OnMouseMove(double x, double y) { return false; }
	virtual bool OnMouseDown(int nButton) { return false; }
	virtual bool OnMouseUp(int nButton) { return false; }

	virtual bool OnKey(int key) { return false; }
	virtual bool OnKeyDown(int key) { return false; }
	virtual bool OnKeyUp(int key) { return false; }

	virtual bool OnWindowResize(int x, int y) { return false; }

};
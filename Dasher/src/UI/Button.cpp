#include "Button.h"
#include "Log.h"

bool Button::OnMouseMove(int x, int y)
{
	LOG_INFO("Mouse Move: {0}, {1}", x, y);
	return false;
}
bool Button::OnMouseDown(int nButton)
{
	LOG_INFO("Mouse down: {0}", nButton);
	return false;

}
bool Button::OnMouseUp(int nButton)
{
	LOG_INFO("Mouse up: {0}", nButton);
	return false;

}
			
bool Button::OnKey(int key)					 
{
	LOG_INFO("Key Press: {0}", key);
	return false;

}
bool Button::OnKeyDown(int key)
{
	LOG_INFO("Key down: {0}", key);
	return false;

}
bool Button::OnKeyUp(int key)
{
	LOG_INFO("key up: {0}", key);
	return false;

}		
bool Button::OnWindowResize(int x, int y)
{
	LOG_INFO("window resize: {0}, {1}", x, y);
	return false;
}
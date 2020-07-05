#pragma once
#include "Application/Application.h"

extern void WindowResizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight);
extern void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
extern void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
extern void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

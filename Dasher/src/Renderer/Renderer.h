#pragma once
#include "Shapes/BaseShape.h"
#include "Texture.h"

namespace Renderer
{
	extern bool Initialise();
	extern void Cleanup();

	extern void EndScene();

	void DrawShape(const BaseShape& shape);
	void DrawShape(const BaseShape& shape, const Texture& texture);

}
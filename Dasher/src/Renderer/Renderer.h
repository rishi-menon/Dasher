#pragma once
#include "Shapes/BaseShape.h"
#include "Texture.h"

namespace Renderer
{
	extern bool Initialise();
	extern void Cleanup();

	extern void EndScene();

	void DrawShape(BaseShape& shape);
	void DrawShape(BaseShape& shape, const Texture& texture);

}
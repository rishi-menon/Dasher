#pragma once
#include "RendererVertex.h"

namespace RendererShapes
{
	extern unsigned int indexBufferQuad[6];


	extern void Rectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col, RendererVertex outQuad[4]);

}
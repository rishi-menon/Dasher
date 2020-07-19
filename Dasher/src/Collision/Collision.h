#pragma once

#include "Renderer/RendererVertex.h"

namespace Collision
{
	extern bool CheckCollision(const RendererVertex shapeA[], size_t countA, const RendererVertex shapeB[], size_t countB);
}
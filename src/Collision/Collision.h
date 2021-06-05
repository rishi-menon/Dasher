#pragma once

#include "Renderer/RendererVertex.h"

namespace Collision
{
	extern bool CheckCollision(const RendererVertex shapeA[], std::size_t countA, const RendererVertex shapeB[], std::size_t countB);
}
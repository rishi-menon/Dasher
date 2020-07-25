#pragma once
#include "RendererVertex.h"

namespace RendererShapes
{

	enum Shape : unsigned int
	{
		ShapeQuad = 0,
		ShapeTriangleRegular = 1,

		ShapeCount
	};

	extern unsigned int* genericIndexBuffer[(unsigned int)Shape::ShapeCount];
	extern unsigned int genericIndexBufferCount[(unsigned int)Shape::ShapeCount];
	extern unsigned int genericVertexBufferCount[(unsigned int)Shape::ShapeCount];

	extern void Rectangle(RendererVertex outQuad[4], const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col);

	//returns a scaled version of an equilateral triangle. 
	extern void TriangleRegular(RendererVertex outTriangle[3], const glm::vec3& pos, const glm::vec3& size, const glm::vec4& col);
}
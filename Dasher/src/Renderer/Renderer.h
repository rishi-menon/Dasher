#pragma once

#include "glm.hpp"
#include "RendererVertex.h"
class Renderer
{
public:
	static bool Initialise();
	static void Cleanup();
	static void OnWindowResize(int nWidth, int nHeight);

	static void Flush();

	static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col);
	static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col, unsigned int nTexId);

	static void DrawQuadColor(RendererVertex* vertexBuffer, unsigned int nVertexCount, unsigned int* indexBuffer, unsigned int nIndexCount);
	static void DrawQuadTexture(RendererVertex* vertexBuffer, unsigned int nVertexCount, unsigned int* indexBuffer, unsigned int nIndexCount, unsigned int nTextureId);

};
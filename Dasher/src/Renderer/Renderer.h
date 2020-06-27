#pragma once

#include "glm.hpp"

struct Vertex
{
	glm::vec3 m_pos;
	glm::vec4 m_col;
	glm::vec2 m_textureCoord;
};

class Renderer
{
public:
	static bool Initialise();
	static void Cleanup();

	static void Flush();

	static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col);
	static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col, unsigned int nTexId);

	static void DrawQuadColor(Vertex* vertexBuffer, unsigned int nVertexCount, unsigned int* indexBuffer, unsigned int nIndexCount);
	static void DrawQuadTexture(Vertex* vertexBuffer, unsigned int nVertexCount, unsigned int* indexBuffer, unsigned int nIndexCount, unsigned int nTextureId);
};
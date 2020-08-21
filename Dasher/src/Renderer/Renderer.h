#pragma once

#include "glm.hpp"
#include "RendererVertex.h"
#include "RendererShapes.h"

#include <string>

class Font;

class Renderer
{
public:
	static bool Initialise();
	static void Cleanup();
	static void OnWindowResize(int nWidth, int nHeight);

	static void Flush();

	static void DrawRectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col);
	static void DrawRectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col, unsigned int nTexId);

	static void DrawQuadColor(RendererVertex* vertexBuffer, RendererShapes::Shape shape, glm::mat4* transformation = nullptr);
	static void DrawQuadTexture(RendererVertex* vertexBuffer, RendererShapes::Shape shape, unsigned int nTextureId, glm::mat4* transformation = nullptr);

	static void DrawQuadColor(RendererVertex* vertexBuffer, unsigned int nVertexCount, const unsigned int* indexBuffer, unsigned int nIndexCount, glm::mat4* transformation = nullptr);
	static void DrawQuadTexture(RendererVertex* vertexBuffer, unsigned int nVertexCount, const unsigned int* indexBuffer, unsigned int nIndexCount, unsigned int nTextureId, glm::mat4* transformation = nullptr);


	//Text rendering
	static void DrawTextColor(const std::string& text, float PosX, float PosY, float scale, const glm::vec4& col, Font* font = nullptr);

	static void DrawTextColor(const std::string& text, const glm::vec2& pos, float scale, const glm::vec4& col, Font* font = nullptr);

};
#pragma once
#include "glm.hpp"

class Renderer;
class RendererVertex
{
	friend class Renderer;
public:
	RendererVertex(const glm::vec3& pos, const glm::vec4& col);
	RendererVertex(const glm::vec3& pos, const glm::vec4& col, const glm::vec2& texCoords);
	RendererVertex();

	void SetPosCol(const glm::vec3& pos, const glm::vec4& col);
	void SetPosColTex(const glm::vec3& pos, const glm::vec4& col, const glm::vec2& texCoords);

	float GetTexId() const		{ return m_textureId; }
	void  SetTexId(float id)	{ m_textureId = id; }
public:
	glm::vec3 m_pos;
	glm::vec4 m_col;
	glm::vec2 m_textureCoord;

private:
	float m_textureId;
};

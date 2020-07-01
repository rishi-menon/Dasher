#include "RendererVertex.h"

RendererVertex::RendererVertex() :
	m_pos(0, 0, 0),
	m_col(0, 0, 0, 1),
	m_textureCoord(0, 0),
	m_textureId (-1)
{

}
RendererVertex::RendererVertex(const glm::vec3& pos, const glm::vec4& col) :
	m_textureCoord(0, 0),
	m_textureId(-1)
{
	SetPosCol(pos, col);
}
RendererVertex::RendererVertex(const glm::vec3& pos, const glm::vec4& col, const glm::vec2& texCoords) :
	m_textureId(-1)
{
	SetPosColTex(pos, col, texCoords);
}

void RendererVertex::SetPosCol(const glm::vec3& pos, const glm::vec4& col)
{
	m_pos = pos;
	m_col = col;
}
void RendererVertex::SetPosColTex(const glm::vec3& pos, const glm::vec4& col, const glm::vec2& texCoords)
{
	m_pos = pos;
	m_col = col;
	m_textureCoord = texCoords;
}

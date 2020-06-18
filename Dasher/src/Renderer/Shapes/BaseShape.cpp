#include "BaseShape.h"


Vertex::Vertex() :
	m_pos(0.0f, 0.0f, 0.0f),
	m_col(0.0f, 0.0f, 0.0f, 0.0f),
	m_textureCoord(0.0f, 0.0f),
	m_textureId(0)
{
}

Vertex::Vertex (const glm::vec3& pos, const glm::vec4& col, const glm::vec2& texCoord) :
	m_pos (pos),
	m_col (col),
	m_textureCoord(texCoord),
	m_textureId (0)
{
}
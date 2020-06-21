#pragma once

#include <glm.hpp>

struct Vertex
{
	Vertex();
	Vertex(const glm::vec3& pos, const glm::vec4& col, const glm::vec2& texCoord);


	glm::vec3 m_pos;
	glm::vec4 m_col;
	glm::vec2 m_textureCoord;
	float m_textureId;
};

class BaseShape
{

public:
	virtual int GetIndicesCount() const = 0;
	virtual int GetVertexCount()  const = 0;
	virtual unsigned int* GetIndicesBuffer() const = 0;

	virtual const Vertex* GetVertexBuffer() const = 0;
	virtual Vertex*		  GetVertexBuffer() = 0;

protected:
};
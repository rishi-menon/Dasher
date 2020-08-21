
#include "RendererShapes.h"
#include "Renderer.h"
#include "Maths.h"

namespace RendererShapes
{
	static unsigned int indexBufferQuad[6] = { 0, 1, 2, 2, 3, 0 };
	static unsigned int indexBufferTriangle[3] = { 0, 1, 2 };

	unsigned int* genericIndexBuffer[(unsigned int)Shape::ShapeCount] = {indexBufferQuad, indexBufferTriangle };
	unsigned int genericIndexBufferCount[(unsigned int)Shape::ShapeCount] = { 6, 3 };
	unsigned int genericVertexBufferCount[(unsigned int)Shape::ShapeCount] = { 4, 3 };

	//////////////////////////////////////////////////////////////////////////////////////////////
	//										Rectangle											//
	//////////////////////////////////////////////////////////////////////////////////////////////
	void Rectangle(RendererVertex outQuad[4], const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col)
	{
		//Set the position
		float halfx = size.x / 2, halfy = size.y / 2;
		outQuad[0].m_pos = { pos.x - halfx, pos.y - halfy, 0.0 };
		outQuad[1].m_pos = { pos.x + halfx, pos.y - halfy, 0.0 };
		outQuad[2].m_pos = { pos.x + halfx, pos.y + halfy, 0.0 };
		outQuad[3].m_pos = { pos.x - halfx, pos.y + halfy, 0.0 };

		outQuad[0].m_textureCoord = { 0.0, 0.0 };
		outQuad[1].m_textureCoord = { 1.0, 0.0 };
		outQuad[2].m_textureCoord = { 1.0, 1.0 };
		outQuad[3].m_textureCoord = { 0.0, 1.0 };

		//set col
		for (int i = 0; i < 4; i++)
		{
			outQuad[i].m_col = col;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//										Triangle											//
	//////////////////////////////////////////////////////////////////////////////////////////////
	void TriangleRegular(RendererVertex outTriangle[3], const glm::vec3& pos, const glm::vec3& size, const glm::vec4& col)
	{
		//pos describes the position of a point at which the shape is created. The shape need not be created with the center at the pos. 'pos' describes the position of this point in the polygon
		const glm::vec4 offset = { 0.0, 0.0, 0.0, 0.0 };

		//The w component must be 1 for it to translate correctly
		glm::vec4 baseCoordinates[3] = {
			{ -1.0, 0.0, 0.0, 1.0 },
			{ 1.0, 0.0, 0.0, 1.0 },
			{ 0.0, 1.73, 0.0, 1.0 }
		};

		glm::mat4 matScale;
		glm::mat4 matUnity = glm::mat4(1.0f);
		matScale = glm::scale(matUnity, size);

		glm::vec3 offsetScaled = matScale * offset;
		
		//first scale the object, then move it to its current position plus the offset
		glm::mat4 matModel = glm::translate<float>(glm::mat4(1.0f), pos - offsetScaled) * matScale;


		glm::vec3 vertexPos = matModel * baseCoordinates[0];
		outTriangle[0].SetPosColTex(vertexPos, col, { 0.0f, 0.0f });

		vertexPos = matModel * baseCoordinates[1];
		outTriangle[1].SetPosColTex(vertexPos, col, { 1.0f, 0.0f });


		vertexPos = matModel * baseCoordinates[2];
		outTriangle[2].SetPosColTex(vertexPos, col, { 0.5f, 1.0f });
	}
}
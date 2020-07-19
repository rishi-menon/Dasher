
#include "RendererShapes.h"
#include "Renderer.h"

namespace RendererShapes
{
	unsigned int indexBufferQuad[6] = { 0, 1, 2, 2, 3, 0 };
	
	void Rectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col, RendererVertex outQuad[4])
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

		//set col and tex id
		for (int i = 0; i < 4; i++)
		{
			outQuad[i].m_col = col;
		}
	}
}
#include "Collision.h"

namespace Collision
{

	//Checks Axis theorem for all edges of A with B
	static bool CheckCollisionHelper(const RendererVertex shapeA[], std::size_t countA, const RendererVertex shapeB[], std::size_t countB)
	{
		for (std::size_t i = 0; i < countA; i++)
		{		
			const glm::vec2 dirToNextVertex = shapeA[(i + 1) % countA].m_pos - shapeA[i].m_pos;
			const glm::vec2 axisNormal = { -dirToNextVertex.y, dirToNextVertex.x };	//this vector will be normal to the line segment joining the two vertices

			float minA = FLT_MAX, maxA = -FLT_MAX;
			for (std::size_t j = 0; j < countA; j++)
			{
				float dotProduct = axisNormal.x * shapeA[j].m_pos.x + axisNormal.y * shapeA[j].m_pos.y;

				minA = glm::min(minA, dotProduct);
				maxA = glm::max(maxA, dotProduct);
			}

			float minB = FLT_MAX, maxB = -FLT_MAX;
			for (std::size_t j = 0; j < countB; j++)
			{
				float dotProduct = axisNormal.x * shapeB[j].m_pos.x + axisNormal.y * shapeB[j].m_pos.y;

				minB = glm::min(minB, dotProduct);
				maxB = glm::max(maxB, dotProduct);
			}

			if (!(maxB >= minA && maxA >= minB))
			{
				//There is no overlap between the projections
				return false;
			}
		}
		return true;
	}

	//Uses seperating axis theorem to check collision
	bool CheckCollision(const RendererVertex shapeA[], std::size_t countA, const RendererVertex shapeB[], std::size_t countB)
	{
		if (shapeA == shapeB)
			return true;

		//To Do: Add some optimization code here to speed up collision checks ?


		if (countA < countB)
		{
			//Check polygon A first and then B
			return CheckCollisionHelper(shapeA, countA, shapeB, countB) &&
				CheckCollisionHelper(shapeB, countB, shapeA, countA);
		}
		else
		{
			//Check polygon B first and then A
			return CheckCollisionHelper(shapeB, countB, shapeA, countA) &&
				CheckCollisionHelper(shapeA, countA, shapeB, countB);
				
		}

	}
}
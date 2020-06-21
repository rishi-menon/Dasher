#pragma once

#include "BaseShape.h"
#include "Log.h"

class QuadShape : public BaseShape
{
public:
	QuadShape() {}

	virtual int GetIndicesCount() const override { return 6; }
	virtual int GetVertexCount()  const override { return 4; }
	virtual unsigned int* GetIndicesBuffer() const override { return m_baseIndiceBuffer; };
	virtual const Vertex* GetVertexBuffer()  const override { return m_vertices; }
	virtual Vertex*		  GetVertexBuffer()  override	    { return m_vertices; }

	inline Vertex& operator[] (int i) 
	{ ASSERT(i < 4, "Error: Index out of bounds"); return m_vertices[i]; }


protected:
	static unsigned int* m_baseIndiceBuffer;
	Vertex m_vertices[4];
};
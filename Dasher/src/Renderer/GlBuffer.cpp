#include "GlBuffer.h"

#include "GL/glew.h"
#include "Log.h"

static const unsigned int s_nInvalidId = 0xffffffff;

GlBuffer::GlBuffer() :
	m_nRendererId (s_nInvalidId),
	m_nSize(0)
{
}

GlBuffer::~GlBuffer()
{
	ASSERT(m_nRendererId == s_nInvalidId, "Buffer did not cleanup properly");
}

void GlBuffer::Cleanup()
{
	glcall(glDeleteBuffers(1, &m_nRendererId));
	m_nRendererId = s_nInvalidId;

}


void GlBuffer::Create(unsigned int nTarget, int nSize, const void* data, unsigned int glUsageHint)
{
	ASSERT(m_nRendererId == s_nInvalidId, "Buffer already exists");
	m_nSize = nSize;
	m_nTarget = nTarget;
	
	glcall(glGenBuffers(1, &m_nRendererId));
	glcall(glBindBuffer(nTarget, m_nRendererId));
	glcall(glBufferData(nTarget, nSize, data, glUsageHint));
}

void GlBuffer::AddLayout(unsigned int nIndex, int nComponents, unsigned int nDataType, bool bNormalised, int stride, int pointer)
{
	//Be sure To BIND the buffer before calling AddLayout
	glcall(glEnableVertexAttribArray(nIndex));
	glcall(glVertexAttribPointer(nIndex, nComponents, nDataType, bNormalised ? GL_TRUE : GL_FALSE, stride, (const void*)pointer));
}

void GlBuffer::UpdateData(int offset, int size, const void* data)
{
	glcall(glBindBuffer(m_nTarget, m_nRendererId));
	glcall(glBufferSubData(m_nTarget, (GLintptr)offset, size, data));
}
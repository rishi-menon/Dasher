#pragma once
class GlBuffer
{
public:
	GlBuffer();
	~GlBuffer();

	int GetSize() const { return m_nSize; }
	unsigned int GetRendererId() const { return m_nRendererId; }

	void Create(unsigned int nTarget, int nSize, const void* data, unsigned int glUsageHint);
	void AddLayout(unsigned int nIndex, int nComponents, unsigned int nDataType, bool bNormalised, int stride, int pointer);

	void UpdateData(int offset, int size, const void* data);

	void Cleanup();
private:
	unsigned int m_nRendererId;
	int m_nSize;
	unsigned int m_nTarget;
};


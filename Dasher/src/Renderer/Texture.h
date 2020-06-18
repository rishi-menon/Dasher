#pragma once

class Texture
{
public:
	Texture();
	~Texture();

	unsigned int GetRendererId() const { return m_nRendererId; }
	unsigned int GetWidth() const { return m_nWidth; }
	unsigned int GetHeight() const { return m_nHeight; }

	void LoadTexture(const char* const strPath);
	void LoadTexture(unsigned char bufferRgba[], int nSizeX, int nSizeY);
private:
	unsigned int m_nRendererId;
	unsigned int m_nWidth, m_nHeight;

};
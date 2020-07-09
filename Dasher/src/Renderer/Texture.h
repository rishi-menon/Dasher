#pragma once

struct TextureProperties
{
	TextureProperties(int nMinFilter, int nMaxFilter, int nWrapS, int nWrapT);

	int m_nMinFilter;
	int m_nMaxFilter;
	int m_nWrapS;
	int m_nWrapT;
};
class Texture
{
	Texture();
	~Texture();
public:

	static unsigned int LoadTexture(const char* const strPath, unsigned int* nOutWidth, unsigned int* nOutHeight, TextureProperties props = TextureProperties (0x2601/*GL LINEAR*/, 0x2601, 0x812F /*GL_CLAMP_TO_EDGE*/, 0x812F));

	static unsigned int LoadTexture(unsigned char bufferRgba[], int nSizeX, int nSizeY, TextureProperties props = TextureProperties(0x2601/*GL LINEAR*/, 0x2601, 0x812F /*GL_CLAMP_TO_EDGE*/, 0x812F));

private:
};
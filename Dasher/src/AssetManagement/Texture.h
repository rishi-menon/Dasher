#pragma once

struct TextureProperties
{
	TextureProperties(int nMinFilter, int nMaxFilter, int nWrapS, int nWrapT);

	int m_nMinFilter;
	int m_nMaxFilter;
	int m_nWrapS;
	int m_nWrapT;
};

struct TextureDimensions
{
	int width, height, bpp;
	unsigned char* buffer;
};
class Texture
{
	Texture();
	~Texture();
public:

	static unsigned int LoadTexture(const char* const strPath, TextureDimensions* outDimensions = nullptr, TextureProperties props = TextureProperties (0x2601/*GL LINEAR*/, 0x2601, 0x812F /*GL_CLAMP_TO_EDGE*/, 0x812F));

	//This does not deallocate the texture image buffer  once its finished
	static unsigned int LoadTexturePreserve(const char* const strPath, TextureDimensions& outDimensions, TextureProperties props = TextureProperties(0x2601/*GL LINEAR*/, 0x2601, 0x812F /*GL_CLAMP_TO_EDGE*/, 0x812F));

	static unsigned int LoadTextureBuffer(const unsigned char bufferRgba[], int nSizeX, int nSizeY, TextureProperties props = TextureProperties(0x2601/*GL LINEAR*/, 0x2601, 0x812F /*GL_CLAMP_TO_EDGE*/, 0x812F));

	//deallocates the memory allocated while stb_image loads an image
	static void FreeImageBuffer(unsigned char* buffer);
	static void DeleteTexture(unsigned int texId);

private:
};

#include "Texture.h"
#include <GL/glew.h>
#include <stb_image.h>

#include "Log.h"


TextureProperties::TextureProperties(int nMinFilter, int nMaxFilter, int nWrapS, int nWrapT) :
	m_nMinFilter (nMinFilter),
	m_nMaxFilter (nMaxFilter),
	m_nWrapS(nWrapS),
	m_nWrapT(nWrapT)
{
}

Texture::Texture()
{
}

Texture::~Texture()
{
}

unsigned int Texture::LoadTexture(const char* const strPath, TextureDimensions* outDimensions, TextureProperties props)
{
	TextureDimensions dimension;
	unsigned int texId = LoadTexturePreserve(strPath, dimension, props);
	if (dimension.buffer)
	{
		stbi_image_free(dimension.buffer);
		dimension.buffer = nullptr;
		if (outDimensions) { *outDimensions = dimension; }
	}
	return texId;
}

//This does not deallocate the buffer, The caller has to store and keep track and eventually call Texture::Free
unsigned int Texture::LoadTexturePreserve(const char* const strPath, TextureDimensions& outDimensions, TextureProperties props)
{
	stbi_set_flip_vertically_on_load(1);
	int bpp;
	outDimensions.buffer = stbi_load(strPath, &outDimensions.width, &outDimensions.height, &outDimensions.bpp, 4);

	//Assume that we read a 3x3 image, the index of each pixel location as seen normally would be as follows
	//	6, 7, 8
	//  3, 4, 5
	//	0, 1, 2
	// ie, the bottom left of the image is the 0th index of the array and then the pixel to its right would be at index 1 (or technically index 4 because index 0 contains rgba)

	if (outDimensions.buffer)
	{
		unsigned int nOutId = Texture::LoadTextureBuffer(outDimensions.buffer, outDimensions.width, outDimensions.height, props);
		return nOutId;
	}
	else
	{
		outDimensions = { 0, 0, 0, nullptr };
		ASSERT(false, "Could not load texture {0}", strPath);
		return 0xffff;
	}
}
unsigned int Texture::LoadTextureBuffer(const unsigned char bufferRgba[], int nSizeX, int nSizeY, TextureProperties props)
{
	unsigned int nOutId;
	glcall(glGenTextures(1, &nOutId));
	glcall(glActiveTexture(GL_TEXTURE0 + 0));
	glcall(glBindTexture(GL_TEXTURE_2D, nOutId));


	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, props.m_nMinFilter/*GL_LINEAR*/));
	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, props.m_nMaxFilter/*GL_LINEAR*/));
	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, props.m_nWrapS/*GL_CLAMP_TO_EDGE*/));
	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, props.m_nWrapT/*GL_CLAMP_TO_EDGE*/));
	

	glcall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nSizeX, nSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferRgba));
	return nOutId;
}

void Texture::FreeImageBuffer(unsigned char* buffer)
{
	ASSERT(buffer, "Texture buffer was empty");
	stbi_image_free(buffer);
}
void Texture::DeleteTexture(unsigned int texId)
{
	glcall(glDeleteTextures(1, &texId));
}
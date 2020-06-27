
#include "Texture.h"
#include <GL/glew.h>
#include <stb_image.h>

#include "Log.h"



static const int s_nInvalidId = 0xffffffff;

Texture::Texture()
{
}

Texture::~Texture()
{
	//glDeleteTextures(1, &m_nRendererId);
	//m_nRendererId = s_nInvalidId;
}

unsigned int Texture::LoadTexture(const char* const strPath, unsigned int* nOutWidth, unsigned int* nOutHeight)
{
	stbi_set_flip_vertically_on_load(1);
	int x, y, bpp;
	unsigned char* buffer = stbi_load(strPath, &x, &y, &bpp, 4);

	if (buffer)
	{
		if (nOutWidth) *nOutWidth = x;
		if (nOutHeight) *nOutHeight = y;

		unsigned int nOutId = Texture::LoadTexture(buffer, x, y);

		stbi_image_free(buffer);
		return nOutId;
	}
	else
	{
		ASSERT(false, "Could not load texture {0}", strPath);
		return 0xffff;
	}
}
unsigned int Texture::LoadTexture(unsigned char bufferRgba[], int nSizeX, int nSizeY)
{
	unsigned int nOutId;
	glcall(glGenTextures(1, &nOutId));
	glcall(glActiveTexture(GL_TEXTURE0 + 0));
	glcall(glBindTexture(GL_TEXTURE_2D, nOutId));

	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	glcall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nSizeX, nSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferRgba));
	return nOutId;
}
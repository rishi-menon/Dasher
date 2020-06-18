
#include "Texture.h"
#include <GL/glew.h>
#include <stb_image.h>

#include "Log.h"



static const int s_nInvalidId = 0xffffffff;

Texture::Texture() :
	m_nRendererId (s_nInvalidId),
	m_nWidth(0),
	m_nHeight(0)
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_nRendererId);
	//m_nRendererId = s_nInvalidId;
}

void Texture::LoadTexture(const char* const strPath)
{
	ASSERT(m_nRendererId == s_nInvalidId, "Cannot load texture {0} as another texture already exists", strPath);

	stbi_set_flip_vertically_on_load(1);

	int x, y, bpp;
	unsigned char* buffer = stbi_load(strPath, &x, &y, &bpp, 4);
	LoadTexture(buffer, x, y);
	if (buffer)
	{
		stbi_image_free(buffer);
	}
}

void Texture::LoadTexture(unsigned char bufferRgba[], int nSizeX, int nSizeY)
{
	ASSERT(m_nRendererId == s_nInvalidId, "Cannot load texture as another texture already exists");

	if (!bufferRgba) { return; }

	m_nWidth = nSizeX;
	m_nHeight = nSizeY;

	glcall(glGenTextures(1, &m_nRendererId));
	glcall(glActiveTexture(GL_TEXTURE0 + 0));
	glcall(glBindTexture(GL_TEXTURE_2D, m_nRendererId));

	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	glcall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nSizeX, nSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferRgba));
}

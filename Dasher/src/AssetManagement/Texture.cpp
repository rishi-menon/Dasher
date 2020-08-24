
#include "Texture.h"
#include <GL/glew.h>
#include <stb_image.h>
#include <unordered_map>
#include "Log.h"

static std::unordered_map<StandardTexture, unsigned int> mapStandardTexture;

void StandardTextureInit()
{
	//To do: This function takes almost a good 4 seconds or so... Try using multiple threads in the future

	mapStandardTexture.reserve(10);

	//Background images have to be loaded with GL_REPEAT so that they loop back
	unsigned int id = Texture::LoadTexture("Assets\\Textures\\External\\bg1.png", nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));
	mapStandardTexture.emplace(StandardTexture::Background1, id);

	id = Texture::LoadTexture("Assets\\Textures\\External\\bg2.png", nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));
	mapStandardTexture.emplace(StandardTexture::Background2, id);

	id = Texture::LoadTexture("Assets\\Textures\\External\\bg3.png", nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));
	mapStandardTexture.emplace(StandardTexture::Background3, id);

	id = Texture::LoadTexture("Assets\\Textures\\External\\bg4.png", nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));
	mapStandardTexture.emplace(StandardTexture::Background4, id);

}
unsigned int GetStandardTextureId(StandardTexture type)
{
	std::unordered_map<StandardTexture, unsigned int>::iterator it = mapStandardTexture.find(type);

	if (it == mapStandardTexture.end())
	{
		ASSERT(false, "Standard texture was not loaded");
		return (unsigned int)(-1);
	}
	else
	{
		return it->second;
	}
}
void StandardTextureCleanup()
{
	for (std::pair<StandardTexture, unsigned int> pair : mapStandardTexture)
	{
		Texture::DeleteTexture(pair.second);
	}
}


////////////////////////////////////////////////////////

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
		return (unsigned int)(-1);
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
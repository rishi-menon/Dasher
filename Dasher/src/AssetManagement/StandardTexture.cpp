#include "StandardTexture.h"
#include <unordered_map>
#include "AssetManagement/Texture.h"
#include <GL/glew.h>
#include "Log.h"

static std::unordered_map<StandardTexture, unsigned int> mapStandardTexture;

bool StandardTextureInit()
{
	//To do: This function takes almost a good 4 seconds or so... Try using multiple threads in the future

	mapStandardTexture.reserve(10);
	

	//Background images have to be loaded with GL_REPEAT so that they loop back
	unsigned int id = Texture::LoadTexture("Assets/Textures/External/bg1.png", nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));
	mapStandardTexture.emplace(StandardTexture::Background1, id);

	if (id == (unsigned int)(-1)) 
	{ 
		LOG_CLIENT_ERROR("Could not find image: Assets/Textures/External/bg1.png");
		return false; 
	}
	
	id = Texture::LoadTexture("Assets/Textures/External/bg2.png", nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));
	mapStandardTexture.emplace(StandardTexture::Background2, id);

	if (id == (unsigned int)(-1))
	{
		LOG_CLIENT_ERROR("Could not find image: Assets/Textures/External/bg2.png");
		return false;
	}

	id = Texture::LoadTexture("Assets/Textures/External/bg3.png", nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));
	mapStandardTexture.emplace(StandardTexture::Background3, id);
	if (id == (unsigned int)(-1))
	{
		LOG_CLIENT_ERROR("Could not find image: Assets/Textures/External/bg3.png");
		return false;
	}

	id = Texture::LoadTexture("Assets/Textures/External/bg4.png", nullptr, TextureProperties(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));
	mapStandardTexture.emplace(StandardTexture::Background4, id);
	if (id == (unsigned int)(-1))
	{
		LOG_CLIENT_ERROR("Could not find image: Assets/Textures/External/bg4.png");
		return false;
	}

	/////////////////////////////////////////////////////////////////////////

	id = Texture::LoadTexture("Assets/Textures/SpecialEffect0.png");
	mapStandardTexture.emplace(StandardTexture::SpecialEffect0, id);
	if (id == (unsigned int)(-1))
	{
		LOG_CLIENT_ERROR("Could not find image: Assets/Textures/SpecialEffect0.png");
		return false;
	}

	return true;
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
		if (pair.second != (unsigned int)(-1))
		{
			Texture::DeleteTexture(pair.second);
		}
	}
}
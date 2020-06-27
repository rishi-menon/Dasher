#pragma once

class Texture
{
	Texture();
	~Texture();
public:

	static unsigned int LoadTexture(const char* const strPath, unsigned int* nOutWidth, unsigned int* nOutHeight);
	static unsigned int LoadTexture(unsigned char bufferRgba[], int nSizeX, int nSizeY);

private:
};
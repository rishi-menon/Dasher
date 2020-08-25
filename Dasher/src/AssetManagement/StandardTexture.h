#pragma once

enum class StandardTexture : int
{
	None = -1,
	Background1 = 0,
	Background2,
	Background3,
	Background4,

	SpecialEffect0,

	Count		//Do not use
};

extern bool StandardTextureInit();
extern unsigned int GetStandardTextureId(StandardTexture type);
extern void StandardTextureCleanup();

#include "DefaultSpawnerFunc.h"
#include "Maths.h"
#include "Application/Application.h"

float g_fBlockSpeed = 400;

static constexpr int s_nBlockColCount = 4;
static const glm::vec4 s_standardCol[s_nBlockColCount] =
{
	//blue
	{ 0.0f, 0.4f, 0.79f, 1.0f },
	{0.0f, 15.0f/ 255.0f, 150.0f/255.0f, 1.0f},

	//red
	{1.0f, 65.0f / 255.0f, 65.0f / 255.0f, 1.0f },
	{163.0f/255.0f, 0.0f, 0.0f, 1.0f }

};
static const glm::vec2 s_standardPhase[s_nBlockColCount] =
{
	{0.05,0.3},
	{0.05,0.3},
	{0.7, 0.95 },
	{0.7, 0.95 }
};

static void CreateBlocksGeneric(CircularQueue<Block>& blocks, double& nextSpawnTime, double curPhase, bool* bDirections, int nBlockCount);

static void CreatePhasableOnly(CircularQueue<Block>& blocks, double& nextSpawnTime, double curPhase);

void DefaultSpawnerFunc(CircularQueue<Block>& blocks, double& nextSpawnTime, double curPhase)
{
	constexpr float fPercPhaseOnly = 10.0f;
	constexpr float fPercGroup3 = 60.0f + fPercPhaseOnly;	//65% chance of creating a 3 group block
	constexpr float fPercGroup4 = 30.0f + fPercGroup3;	
	
	//for percentage, the sum of all the variables should be 100, but this formula would work for non 100 sum numbers as well. (It would be 65 parts of the total chance of happening instead of 65%)

	float fPart = Random::Rand() * fPercGroup4;

	if (fPart <= fPercPhaseOnly)
	{
		CreatePhasableOnly(blocks, nextSpawnTime, curPhase);
	}
	else 
	{
		int nCount;
		if (fPart < fPercGroup3) { nCount = 3; }
		else { nCount = 4; }

		//4 because thats the maximum number of blocks at a time. True: block gets spawned at the top position
		bool bDirections[4];

		for (int i = 0; i < nCount; i++)
		{
			bDirections[i] = (Random::Rand() >= 0.5f);
		}

		CreateBlocksGeneric(blocks, nextSpawnTime, curPhase, bDirections, nCount);
	}
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


//Sets velocity, shape, color, isPhasable and PhaseRange
void SetGenericProperties(Block& block, double curPhase)
{
	block.velocity = { -g_fBlockSpeed, 0, 0 };
	block.shape = RendererShapes::ShapeTriangleRegular;

	int index = static_cast<int>(Random::Rand() * s_nBlockColCount);
	if (index == s_nBlockColCount) { index--; }

	block.color = s_standardCol[index];
	block.phaseRange = s_standardPhase[index];

	if (block.phaseRange.x <= curPhase && block.phaseRange.y >= curPhase)
	{
		block.isPhasable = true;
		block.color.a = 0.5f;
	}
	else
	{
		block.isPhasable = false;
		//phase is 1.0f by default
	}
}


//bDirections should be an array of length nBlockCount. if bDirections is true then the block is created on the top, else at the bottom
static void CreateBlocksGeneric(CircularQueue<Block>& blocks, double& nextSpawnTime, double curPhase, bool* bDirections, int nBlockCount)
{
	const glm::vec2 sizeX = { 80, 100 };
	const glm::vec2 sizeY = { 300, 500 };
	constexpr float fDistanceBwSpawns = 550.0f;
	constexpr float fDistToNextSpawn = 800.0f;

	const int width = Application::GetWidth();
	const int nPosDown = 2;
	const int nPosUp = Application::GetHeight() - 2;

	for (char i = 0; i < nBlockCount; i++)
	{
		Block& curBlock = *blocks.Push();

		if (bDirections[i])
		{
			//spawn the block up
			curBlock.position = { width + i * fDistanceBwSpawns,
									nPosUp, 0.1f };

			curBlock.scale = { Random::Rand(sizeX.x, sizeX.y),
							-Random::Rand(sizeY.x, sizeY.y), 1.0f };
		}
		else
		{
			//bottom
			curBlock.position = { width + i * fDistanceBwSpawns,
						nPosDown, 0.1f };

			curBlock.scale = { Random::Rand(sizeX.x, sizeX.y),
							Random::Rand(sizeY.x, sizeY.y), 1.0f };
		}
		SetGenericProperties(curBlock, curPhase);
	}

	nextSpawnTime = Application::GetGameTime() + ((nBlockCount-1) * fDistanceBwSpawns + fDistToNextSpawn) / g_fBlockSpeed;
}

////////////////////////////////////////////////////////////////////
//                                                                //
////////////////////////////////////////////////////////////////////

static void CreatePhasableOnly(CircularQueue<Block>& blocks, double& nextSpawnTime, double curPhase)
{
	const glm::vec2 sizeX = { 80, 100 };
	constexpr float fDistanceBeforeSpawn = 400.0f;
	constexpr float fDistToNextSpawn = 900.0f;
	
	const int width = Application::GetWidth();

	int index = static_cast<int>(Random::Rand() * s_nBlockColCount);	//for color and phase range
	if (index == s_nBlockColCount) { index--; }

	 
	float sizeY1 = Random::Rand(350 - 150, 350 + 150);
	float sizeY2 = 700.0f - sizeY1;	//700 fills the entire screen 

	for (char i = 0; i < 2; i++)
	{
		Block& curBlock = *blocks.Push();

		if (i % 2)
		{
			//spawn the block up
			curBlock.position = { width + fDistanceBeforeSpawn,
									Application::GetHeight() - 2, 0.1f };
			curBlock.scale = { Random::Rand(sizeX.x, sizeX.y),
							-sizeY1, 1.0f };
		}
		else
		{
			//bottom
			curBlock.position = { width + fDistanceBeforeSpawn,
								2, 0.1f };
			curBlock.scale = { Random::Rand(sizeX.x, sizeX.y),
							sizeY2, 1.0f };
		}
		

		curBlock.velocity = { -g_fBlockSpeed, 0, 0 };
		curBlock.shape = RendererShapes::ShapeTriangleRegular;
		curBlock.color = s_standardCol[index];
		curBlock.phaseRange = s_standardPhase[index];

		if (curBlock.phaseRange.x <= curPhase && curBlock.phaseRange.y >= curPhase)
		{
			curBlock.isPhasable = true;
			curBlock.color.a = 0.5f;
		}
		else
		{
			curBlock.isPhasable = false;
			//phase is 1.0f by default
		}
	}

	nextSpawnTime = Application::GetGameTime() + (fDistanceBeforeSpawn + fDistToNextSpawn) / g_fBlockSpeed;
}

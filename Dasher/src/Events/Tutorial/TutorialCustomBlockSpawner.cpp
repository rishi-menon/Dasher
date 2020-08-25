
#include "TutorialCustomBlockSpawner.h"
#include "TutorialBlockSpawnerLayer.h"
#include "Application/Application.h"

TutorialBlockSpawnerLayer* g_currentInstance = nullptr;

void TutorialBlockSpawner(Block& newBlock, double& nextSpawnTime, double curPhase)
{
	ASSERT(g_currentInstance, "Current instance of tutorial layer was null");

	TutorialStage currentStage = g_currentInstance->GetCurrentStage();
	switch (currentStage)
	{
		case TutorialStage::PlayMove:
		{
			ASSERT(false, "This should not have happened");
			newBlock.position = { -10.0f, 0.0f, 0.1f };	//Itll get destoryed immediately
			newBlock.scale = { 1, 1, 1 };

			newBlock.velocity = { -400, 0, 0 };
			newBlock.color = { 0.0,0.4,0.79,1.0 };
			newBlock.shape = RendererShapes::ShapeTriangleRegular;
			newBlock.phaseRange = { 2.0, 3.0 };//impossible range
			newBlock.isPhasable = false;
			nextSpawnTime = -10;	//forces an object to never spawn
			break;
		}
		case TutorialStage::PlayFriendlySpikes:
		{
			int blockCount = g_currentInstance->GetBlockCount();
			constexpr int nMaxBlocks = 4;
			int width = Application::GetWidth();
			int posY;
			
			if (blockCount % 2)
			{
				//Down
				posY = 2;
				newBlock.scale = { 90, 400, 1 };
			}
			else
			{
				posY = Application::GetHeight() - 2;
				newBlock.scale = { 90, -400, 1 };
			}

			newBlock.position = { width + (blockCount - 1) * 700, posY, 0.1f };
			newBlock.velocity = { -400, 0, 0 };
			newBlock.color = { 0.0,0.4,0.79,1.0 };
			newBlock.shape = RendererShapes::ShapeTriangleRegular;
			newBlock.phaseRange = { 2.0, 3.0 };//impossible range
			newBlock.isPhasable = false;

			if (blockCount < nMaxBlocks)
			{
				nextSpawnTime = 0.0;	//Spawn immediately
			}
			else
			{
				nextSpawnTime = -10;	//forces an object to never spawn
			}
			break;
		}
		case TutorialStage::PlayWarningCollision:
		{
			int blockCount = g_currentInstance->GetBlockCount();
			constexpr int nMaxBlocks = 2;
			int width = Application::GetWidth();
			int posY;

			if (blockCount % 2)
			{
				//Down
				posY = 2;
				newBlock.scale = { 90, 350, 1 };
			}
			else
			{
				posY = Application::GetHeight() - 2;
				newBlock.scale = { 90, -350, 1 };
			}

			newBlock.position = { width, posY, 0.1f };
			newBlock.velocity = { -400, 0, 0 };
			newBlock.color = { 0.0,0.4,0.79,1.0 };
			newBlock.shape = RendererShapes::ShapeTriangleRegular;
			newBlock.phaseRange = { 2.0, 3.0 };//impossible range
			newBlock.isPhasable = false;

			if (blockCount < nMaxBlocks)
			{
				nextSpawnTime = 0.0;	//Spawn immediately
			}
			else
			{
				nextSpawnTime = -10;	//forces an object to never spawn
			}
			break;
		}
		case TutorialStage::PlayPhasing:
		{
			int blockCount = g_currentInstance->GetBlockCount();	//ranges from 1-6 not 0-6
			constexpr int nMaxBlocks = 6;
			int width = Application::GetWidth();
			int posY;

			int nGroup = (blockCount-1) / 2;	//ranges from 0-2

			if (blockCount % 2)
			{
				//Down
				posY = 2;
				newBlock.scale = { 90, 350, 1 };
			}
			else
			{
				posY = Application::GetHeight() - 2;
				newBlock.scale = { 90, -350, 1 };
			}

			newBlock.position = { width + 750* nGroup, posY, 0.1f };
			newBlock.velocity = { -400, 0, 0 };
			newBlock.color = { 0.0,0.4,0.79,1.0 };
			newBlock.shape = RendererShapes::ShapeTriangleRegular;
			
			switch (nGroup)
			{
				case 0:
				{
					newBlock.phaseRange = { 0.0, 0.2 };//impossible range
					break;
				}
				case 1:
				{
					newBlock.phaseRange = { 0.8, 1.0 };//impossible range
					break;
				}
				case 2:
				{
					newBlock.phaseRange = { 0.35, 0.65 };//impossible range
					break;
				}
			}
			
			newBlock.isPhasable = (newBlock.phaseRange.x <= curPhase && newBlock.phaseRange.y >= curPhase);

			if (blockCount < nMaxBlocks)
			{
				nextSpawnTime = 0.0;	//Spawn immediately
			}
			else
			{
				nextSpawnTime = -10;	//forces an object to never spawn
			}
			break;
		}

		default:
		{
			ASSERT(false, "Something went wrong here..");
			break;
		}
	}
#if 0
	//spawn blocks
	const glm::vec2 timeBwSpawn = { 1.4f, 2.2f };

	//size
	const glm::vec2 sizeX = { 80, 100 };
	const glm::vec2 sizeY = { 300, 500 };

	if (Random::Rand() >= 0.5)
	{
		//newBlock Top
		newBlock.position = { Application::GetWidth(), Application::GetHeight() - 2, 0.1 };
		//Negative sign so that the triangle appears upside down
		newBlock.scale = { Random::Rand(sizeX.x, sizeX.y), -Random::Rand(sizeY.x, sizeY.y), 1 };
	}
	else
	{
		//newBlock bottom
		Application* pApp = Application::GetCurrentApp();
		newBlock.position = { pApp->GetWidth(), 2, 0.1 };
		newBlock.scale = { Random::Rand(sizeX.x, sizeX.y), Random::Rand(sizeY.x, sizeY.y), 1 };
	}

	newBlock.velocity = { -400, 0, 0 };
	newBlock.color = { 0.0,0.4,0.79,1.0 };
	newBlock.shape = RendererShapes::ShapeTriangleRegular;
	newBlock.phaseRange = { 0.0, 0.2 };

	if (newBlock.phaseRange.x <= curPhase && newBlock.phaseRange.y >= curPhase)
	{
		newBlock.isPhasable = true;
		newBlock.color.a = 0.5f;
	}
	else
	{
		newBlock.isPhasable = false;
		newBlock.color.a = 1.0f;
	}
	nextSpawnTime = Application::GetGameTime() + Random::Rand(timeBwSpawn.x, timeBwSpawn.y);
#endif
}

#include "TutorialSpawnerFunc.h"
#include "Events/Tutorial/TutorialBlockSpawnerLayer.h"
#include "Application/Application.h"

TutorialBlockSpawnerLayer* g_currentInstance = nullptr;

void TutorialSpawnerFunc(CircularQueue<Block>& blocksQueue, double& nextSpawnTime, double curPhase)
{
	ASSERT(g_currentInstance, "Current instance of tutorial layer was null");

	TutorialStage currentStage = g_currentInstance->GetCurrentStage();
	switch (currentStage)
	{
		case TutorialStage::PlayMove:
		{
			ASSERT(false, "This should not have happened");
			break;
		}
		case TutorialStage::PlayFriendlySpikes:
		{
			constexpr int nMaxBlocks = 4;
			
			int width = Application::GetWidth();
			int posY;

			float scaleY = Application::GetHeight() * 400.0f/1200.0f;
			for (int i = 0; i < nMaxBlocks; i++)
			{
				Block& newBlock = *blocksQueue.Push();
				if (!(i % 2))
				{
					//Down
					posY = 2;
					newBlock.scale = { 90, scaleY, 1 };
				}
				else
				{
					posY = Application::GetHeight() - 2;
					newBlock.scale = { 90, -scaleY, 1 };
				}
				newBlock.position = { width + i * 700, posY, 0.1f };
				newBlock.velocity = { -400, 0, 0 };
				newBlock.color = { 0.0,0.4,0.79,1.0 };
				newBlock.shape = RendererShapes::ShapeTriangleRegular;
				newBlock.phaseRange = { 2.0, 3.0 };//impossible range
				newBlock.isPhasable = false;
			}
			nextSpawnTime = -10;	//forces an object to never spawn
			break;
		}
#if 0
		case TutorialStage::PlayWarningCollision:
		{
			constexpr int nMaxBlocks = 2;
			int width = Application::GetWidth();
			int posY;
			float scaleY = Application::GetHeight() * 350.0f/1200.0f;

			for (int i = 0; i < nMaxBlocks; i++)
			{
				Block& newBlock = *blocksQueue.Push();
				if (!(i % 2))
				{
					//Down
					posY = 2;
					newBlock.scale = { 90, scaleY, 1 };
				}
				else
				{
					posY = Application::GetHeight() - 2;
					newBlock.scale = { 90, -scaleY, 1 };
				}
				newBlock.position = { width, posY, 0.1f };
				newBlock.velocity = { -400, 0, 0 };
				newBlock.color = { 0.0,0.4,0.79,1.0 };
				newBlock.shape = RendererShapes::ShapeTriangleRegular;
				newBlock.phaseRange = { 2.0, 3.0 };//impossible range
				newBlock.isPhasable = false;
			}

			nextSpawnTime = -10;	//forces an object to never spawn
			break;
		}
#endif
		case TutorialStage::PlayPhasing:
		{

			constexpr int nGroups = 3;
			int width = Application::GetWidth();
			int posY;
			float scaleY = Application::GetHeight() * 350.0f/1200.0f;

			for (int j = 0; j < nGroups; j++)
			{
				for (int i = 0; i < 2; i++)
				{
					Block& newBlock = *blocksQueue.Push();
					if (!(i % 2))
					{
						//Down
						posY = 2;
						newBlock.scale = { 90, scaleY, 1 };
					}
					else
					{
						posY = Application::GetHeight() - 2;
						newBlock.scale = { 90, -scaleY, 1 };
					}
					newBlock.position = { width + 1800*j, posY, 0.1f };
					newBlock.velocity = { -400, 0, 0 };
					newBlock.color = { 0.0 + 0.3*j,0.4 - 0.1*j,0.79,1.0 };
					newBlock.shape = RendererShapes::ShapeTriangleRegular;
					
					switch (j)
					{
						case 0:
						{
							newBlock.phaseRange = { 0.0, 0.2 };
							break;
						}
						case 1:
						{
							newBlock.phaseRange = { 0.8, 1.0 };
							break;
						}
						case 2:
						{
							newBlock.phaseRange = { 0.35, 0.65 };
							break;
						}
						default:
						{
							newBlock.phaseRange = { 0.0, 0.2 };
							break;
						}
					}
					
					newBlock.isPhasable = (newBlock.phaseRange.x <= curPhase && newBlock.phaseRange.y >= curPhase);

				}
			}
			nextSpawnTime = -10;	//forces an object to never spawn
			break;
		}

		default:
		{
			ASSERT(false, "Something went wrong here..");
			break;
		}
	}
}
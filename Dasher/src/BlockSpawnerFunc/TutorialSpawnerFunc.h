#pragma once
#include "Events/BlockSpawnerLayer.h"

class TutorialBlockSpawnerLayer;

extern TutorialBlockSpawnerLayer* g_currentInstance;
extern void TutorialSpawnerFunc (CircularQueue<Block>& blocksQueue, double& nextSpawnTime, double curPhase);

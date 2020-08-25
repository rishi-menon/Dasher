#pragma once
#include "Events/BlockSpawnerLayer.h"

class TutorialBlockSpawnerLayer;

extern TutorialBlockSpawnerLayer* g_currentInstance;
extern void TutorialBlockSpawner(Block& newBlock, double& nextSpawnTime, double curPhase);

#pragma once
#include "Events/BlockSpawnerLayer.h"

extern float g_fBlockSpeed;
extern void DefaultSpawnerFunc(CircularQueue<Block>& blocks, double& nextSpawnTime, double curPhase);


#include "BlockSpawnerLayer.h"
#include "Application/Application.h"

#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

BlockSpawnerLayer::BlockSpawnerLayer()
{
	BlockSpawnerLayer::ResetLayer();
}
void BlockSpawnerLayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
}
void BlockSpawnerLayer::ResetLayer()
{

}

void BlockSpawnerLayer::OnStart()
{

}
void BlockSpawnerLayer::OnUpdate(float deltaTime)
{

}
bool BlockSpawnerLayer::OnWindowResize(int width, int height)
{
	return false;
}
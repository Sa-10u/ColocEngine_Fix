#include "GameMain.h"
#include"WorldManager.h"

void GameMain::Init()
{
	WorldManager::Scene->Initialize();
}

void GameMain::Update(float tick)
{
	WorldManager::Scene->Update(tick);
}

void GameMain::Release()
{
}

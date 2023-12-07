#include "WorldManager.h"
#include "IComp.h"

#include"WW_BlackScreen.h"
#include"WW_Play.h"

void WorldManager::ChangeWorld(WLD_ID next)
{
	if (next == NowWorld)	return;

	Scene->Release();
	delete Scene;

	switch (next)
	{
	case WLD_ID::PLAY			:	Scene = new WW_Play; break;

	case WLD_ID::SCENE			:	Scene = nullptr; break;

	case WLD_ID::BLACK_SCREEN	:	Scene = new WW_BlackScreen; break;

	default:	exit(-1);
	}

	Scene->Initialize();
}

void WorldManager::Init()
{
	
}

void WorldManager::Release()
{
}

namespace WorldManager
{
	WLD_ID NowWorld = WLD_ID::BLACK_SCREEN;
	WorldWork* Scene = new WW_BlackScreen;
}


#include "WorldManager.h"
#include "ResourceManager.h"
#include"IComp.h"

void WorldWork::Update(float tick)
{
	Run(tick);
	//-------------
	DataManager::Process(tick);
	DataManager::Sub_Process(tick);
}

void WorldWork::Release()
{
	release();
	//------------

	DataManager::ALL_RESET();
	ResourceManager::ALL_RELEASE_MODEL();
	ResourceManager::ALL_RELEASE_TEX();
}

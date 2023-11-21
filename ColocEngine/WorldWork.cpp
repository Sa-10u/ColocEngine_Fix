#include "WorldManager.h"
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
}

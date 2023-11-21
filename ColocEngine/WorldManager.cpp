#include "WorldManager.h"
#include "IComp.h"

void WorldManager::ChangeWorld(WLD_ID next)
{
	if (next == NowWorld)	return;

	Scene->Release();

	switch (next)
	{
	case WLD_ID::PLAY			:	break;

	case WLD_ID::SCENE			:	break;

	case WLD_ID::BLACK_SCREEN	:	break;
	}
}

void WorldWork::Update(float tick)
{
	Run(tick);	
//-------------
	DataManager::Process(tick);
	DataManager::Sub_Process(tick);
}

void WorldWork::Release()
{
	Delete();
//------------

	DataManager::
}

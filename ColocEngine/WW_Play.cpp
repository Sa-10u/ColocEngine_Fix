#include "WW_Play.h"
#include"S_Draw.h"
#include"BUFFER.h"
#include"ResourceManager.h"

void WW_Play::Initialize()
{
	md = ResourceManager::ModelLoad(L"Re_Meta Knigt.fbx");
	md0 = ResourceManager::ModelLoad(L"Sphere.fbx");
}

void WW_Play::Run(float tick)
{
	S_Draw::Draw(&info, md);

	ObjInfo i;
	i.wld =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		1.5,0,0,1
	};
	//S_Draw::Draw(&i, md0);
}

void WW_Play::release()
{
}

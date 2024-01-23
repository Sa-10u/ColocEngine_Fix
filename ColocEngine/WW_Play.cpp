#include"WW_Play.h"
#include"S_Draw.h"
#include"BUFFER.h"
#include"ResourceManager.h"
#include"C_Trans.h"

void WW_Play::Initialize()
{
	md = ResourceManager::ModelLoad(L"TEST_MK.fbx");
	md0 = ResourceManager::ModelLoad(L"Sphere.fbx");

	ResourceManager::TexLoad(L"default.dds");
}

void WW_Play::Run(float tick)
{
	S_Draw::Draw(&info, md , nullptr ,0);

	C_Trans t0("a");
	t0.pos = Position(-1.0, 0, 0);
	S_Draw::Draw(&t0, md , nullptr ,0);


	ObjInfo i;
	i.wld =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		1.5,0,0,1
	};
	S_Draw::Draw(&i, md0 , nullptr , 0);

	i.wld =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		1.5,-1,0,1
	};
	S_Draw::Draw(&i, md0 , nullptr , 0);
}

void WW_Play::release()
{
}

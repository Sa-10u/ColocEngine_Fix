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
	ResourceManager::TexLoad(L"VAVA.dds");
}

void WW_Play::Run(float tick)
{
	MapBOOL m{};
	MapBOOL m1{};
	m.isD = ResourceManager::TexLoad(L"default.dds");
	m1.isD = ResourceManager::TexLoad(L"default.dds");

	MapBOOL* arr1[] = {&m ,nullptr ,&m1};

	S_Draw::Draw(&info, md ,arr1 ,_countof(arr1));

	C_Trans t0("a");
	t0.pos = Position(-1.0, 0, 0);

	MapBOOL mb{};
	mb.isD = ResourceManager::TexLoad(L"VAVA.dds");

	MapBOOL* arr0 = { &mb };

	S_Draw::Draw(&t0, md , &arr0, 1);


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

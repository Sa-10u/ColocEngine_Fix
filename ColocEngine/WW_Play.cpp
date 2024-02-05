#include"WW_Play.h"
#include"S_Draw.h"
#include"BUFFER.h"
#include"ResourceManager.h"
#include"C_Trans.h"
#include"LightManager.h"

void WW_Play::Initialize()
{
	md = ResourceManager::ModelLoad(L"TEST_MK.fbx");
	md0 = ResourceManager::ModelLoad(L"Plane.fbx");

	ResourceManager::TexLoad(L"default.dds");
	ResourceManager::TexLoad(L"VAVA.dds");
}

void WW_Play::Run(float tick)
{
	MapBOOL m{};
	MapBOOL m1{};
	m.isD = ResourceManager::TexLoad(L"default.dds");
	m1.isD = ResourceManager::TexLoad(L"MK_tex1.png");

	MapBOOL* arr1[] = {&m1 ,&m ,&m , &m ,&m, &m };

	S_Draw::Draw(&info, md ,arr1 ,_countof(arr1));

	C_Trans t0("a");
	t0.pos = Position(-1.0, 0, 0);

	MapBOOL mb{};
	mb.isD = ResourceManager::TexLoad(L"VAVA.dds");

	MapBOOL* arr0 = { &mb };

	S_Draw::Draw(&t0, md , &arr0, 1);

	C_Trans tr("a");


	tr.rot = Rotation(85, 0, 0);
	tr.scale = Scale(.1, .1, .1);
	tr.pos = Position(1, 0, 0);

	static ObjInfo oi = {};
	oi.val0 += 0.01f;
	oi.wld = tr.WLDGetMTX();

	MapBOOL mp = {};
	mp.isD = ResourceManager::TexLoad(L"Water.jpg");
	mp.isN = ResourceManager::TexLoad(L"NormalMap.png");

	MapBOOL* mc[] = {&mp};

	S_Draw::Draw(&oi, md0, mc, 1);
	
}

void WW_Play::release()
{
}

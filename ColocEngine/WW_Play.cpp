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

	auto ind = LightManager::CreateLight<P_Light>();
	LightManager::lights.point.SetColor(ind, Color(1, 1, 1));
	LightManager::lights.point.SetIntensity(ind, 10);
	LightManager::lights.point.SetPosition(ind, Position(1, 3, -.5));
}

void WW_Play::Run(float tick)
{
	MapBOOL m{};
	MapBOOL m1{};
	m.isD = ResourceManager::TexLoad(L"default.dds");
	m1.isD = ResourceManager::TexLoad(L"MK_tex1.png");

	MapBOOL* arr1[] = {&m1 ,&m ,&m , &m ,&m, &m };

	static C_Trans t0("a");
	t0.pos = Position(0.0, 0, 0);
	//t0.rot.y += 0.01;

	info.wld = t0.WLDGetMTX();

	//S_Draw::Draw(&info, md ,arr1 ,_countof(arr1));

	
	MapBOOL mb{};
	mb.isD = ResourceManager::TexLoad(L"VAVA.dds");

	MapBOOL* arr0 = { &mb };

	//S_Draw::Draw(&t0, md , &arr0, 1);

	C_Trans tr("a");


	tr.rot = Rotation(-0.4, 0, 0);
	tr.scale = Scale(.2, .2, .2);
	tr.pos = Position(.5, 0, 0);

	static ObjInfo oi = {};
	oi.val0 += 0.001f;
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

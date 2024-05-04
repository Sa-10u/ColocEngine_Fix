#include"WW_Play.h"
#include"S_Draw.h"
#include"BUFFER.h"
#include"ResourceManager.h"
#include"S_Light.h"

void WW_Play::Initialize()
{
	DataManager::CreateEntity(&p_ent0, "Player");

	DataManager::AddComponent<C_Player>(&(p_ent0->comps), "Body", p_ent0);

	auto lgt = DataManager::AddComponent<A_Light>(&(p_ent0->comps), "A0", p_ent0);
	lgt->SetIntensity(10);
}

void WW_Play::Run(float tick)
{
	
}

void WW_Play::release()
{
}

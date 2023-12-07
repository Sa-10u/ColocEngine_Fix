#include "WW_BlackScreen.h"
#include"C_SetColorBG.h"
#include"ResourceManager.h"
#include"S_Draw.h"
#include"C_Trans.h"

void WW_BlackScreen::Initialize()
{
	DataManager::CreateEntity(&BG, "BG");
	auto i = DataManager::AddComponent<C_SetColorBG>(&BG->comps, "BGchanger", BG);
}

void WW_BlackScreen::Run(float tick)
{
	ResourceManager::ModelLoad(L"Re_Meta Knigt.fbx");
}

void WW_BlackScreen::release()
{
}

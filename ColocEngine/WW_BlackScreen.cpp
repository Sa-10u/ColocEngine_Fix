#include "WW_BlackScreen.h"
#include"C_SetColorBG.h"
#include"ResourceManager.h"
#include"S_Draw.h"

void WW_BlackScreen::Initialize()
{
	DataManager::CreateEntity(&BG, "BG");

	DataManager::AddComponent<C_SetColorBG>(&BG->comps, "BGchanger", BG);
	
	md = ResourceManager::ModelLoad(L"Re_Meta Knigt.fbx");
}

void WW_BlackScreen::Run(float tick)
{
	XMMATRIX mat = XMMatrixIdentity();
	S_Draw::Draw(&mat , md);
}

void WW_BlackScreen::release()
{
}

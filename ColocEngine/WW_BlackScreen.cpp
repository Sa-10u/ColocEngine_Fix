#include "WW_BlackScreen.h"

#include"C_SetColorBG.h"
#include"Input.h"
#include"ResourceManager.h"
#include"S_Draw.h"
#include"C_Trans.h"

void WW_BlackScreen::Initialize()
{
	DataManager::CreateEntity(&BG, "BG");
	DataManager::AddComponent<C_SetColorBG>(&BG->comps, "BGchanger", BG);

	md = ResourceManager::ModelLoad(L"Re_Meta Knigt.fbx");
}

void WW_BlackScreen::Run(float tick)
{
	if (Input_KB::Down(DIK_SPACE))
	{
		WorldManager::ChangeWorld(WLD_ID::PLAY);
	}
	static C_Trans trans("a");

	trans.pos.x += 0.01;
	S_Draw::Draw(&trans, md);
}

void WW_BlackScreen::release()
{
}

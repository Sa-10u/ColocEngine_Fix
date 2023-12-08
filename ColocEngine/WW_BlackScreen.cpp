#include "WW_BlackScreen.h"

#include"C_SetColorBG.h"
#include"Input.h"
#include"ResourceManager.h"
#include"S_Draw.h"
#include"C_Trans.h"
#include"BUFFER.h"

void WW_BlackScreen::Initialize()
{
	DataManager::CreateEntity(&BG, "BG");
	DataManager::AddComponent<C_SetColorBG>(&BG->comps, "BGchanger", BG);
}

void WW_BlackScreen::Run(float tick)
{
	if (Input_KB::Down(DIK_SPACE))
	{
		WorldManager::ChangeWorld(WLD_ID::PLAY);
	}
}

void WW_BlackScreen::release()
{
}

#include "WW_BlackScreen.h"
#include"C_SetColorBG.h"

void WW_BlackScreen::Initialize()
{
	DataManager::CreateEntity(&BG, "BG");

	DataManager::AddComponent<C_SetColorBG>(&BG->comps, "BGchanger", BG);
}

void WW_BlackScreen::Run(float tick)
{

}

void WW_BlackScreen::release()
{
}

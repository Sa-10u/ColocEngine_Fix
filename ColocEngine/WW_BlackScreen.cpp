#include "WW_BlackScreen.h"

#include"Input.h"
#include"ResourceManager.h"
#include"S_Draw.h"
#include"D3D.h"
#include<cmath>
#include<numbers>

void WW_BlackScreen::Initialize()
{
	ptr = &WW_BlackScreen::P1_BlackToWhite;
	val0 = .0f;
	val1 = .0f;

	DataManager::CreateEntity(&e_title , "Title");

	S_Sound::LoadWave(L"loop1.wav", &ad);

	S_Sound::CreateSE(&ad, S_Sound::FLAG::AutoRelease, &c);
}

void WW_BlackScreen::Run(float tick)
{
	(this->*ptr)();
}

void WW_BlackScreen::release()
{
	e_title->Release();
}

WW_BlackScreen::~WW_BlackScreen()
{
	auto i = 0;
}

void WW_BlackScreen::P1_BlackToWhite()
{
	val0 += 0.1f;
	float col = sin(val0 /3.141592);
	PTR_D3D::ptr->SetColorBG(col, col, col, 1);

	if (col < val1)
	{
		PTR_D3D::ptr->SetColorBG(0, 0, 0, 0);
		ptr = &WW_BlackScreen::P2_Title;
		val0 = 0;
		val1 = 0;
	}

	val1 = col;

}

void WW_BlackScreen::P2_Title()
{	
	val1 += 1;
	C_Trans* trans = nullptr;

	if (val1 > 30)
	{
		ptr = &WW_BlackScreen::P3_TitleToSelect;
		val0 = 0;
		val1 = 0;
		PTR_D3D::ptr->SetColorBG(1, 0, 0, 0);
	}

}

void WW_BlackScreen::P3_TitleToSelect()
{
	WorldManager::ChangeWorld(WLD_ID::PLAY);
}

void WW_BlackScreen::P4_Select()
{
}

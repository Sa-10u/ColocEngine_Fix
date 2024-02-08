#include "WW_BlackScreen.h"

#include"C_SetColorBG.h"
#include"Input.h"
#include"ResourceManager.h"
#include"S_Draw.h"
#include"C_Trans.h"
#include"BUFFER.h"

void WW_BlackScreen::Initialize()
{
	ptr = &P1_BlackToWhite;
	val0 = .0f;
}

void WW_BlackScreen::Run(float tick)
{
	this->*ptr();
}

void WW_BlackScreen::release()
{
}

void WW_BlackScreen::P1_BlackToWhite()
{
}

void WW_BlackScreen::P2_Title()
{
}

void WW_BlackScreen::P3_TitleToSelect()
{
}

void WW_BlackScreen::P4_Select()
{
}

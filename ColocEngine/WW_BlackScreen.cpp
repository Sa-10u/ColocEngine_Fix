#include "WW_BlackScreen.h"

#include"C_SetColorBG.h"
#include"Input.h"
#include"ResourceManager.h"
#include"S_Draw.h"
#include"C_Trans.h"
#include"BUFFER.h"
#include"D3D.h"
#include<cmath>
#include<numbers>

void WW_BlackScreen::Initialize()
{
	ptr = &WW_BlackScreen::P1_BlackToWhite;
	val0 = .0f;
	val1 = .0f;
}

void WW_BlackScreen::Run(float tick)
{
	(this->*ptr)();
}

void WW_BlackScreen::release()
{
}

void WW_BlackScreen::P1_BlackToWhite()
{
	val0 += 0.2f;
	float col = sin(val0 /3.141592);
	PTR_D3D::ptr->SetColorBG(col, col, col, 1);

	if (col < val1)
	{
		PTR_D3D::ptr->SetColorBG(0, 0, 0, 0);
		ptr = &WW_BlackScreen::P2_Title;
	}

	val1 = col;

}

void WW_BlackScreen::P2_Title()
{
	//SetTitle Picture;


}

void WW_BlackScreen::P3_TitleToSelect()
{
}

void WW_BlackScreen::P4_Select()
{
}

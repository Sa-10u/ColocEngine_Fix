#include "C_SetColorBG.h"
#include"D3D.h"

void C_SetColorBG::initialize()
{

}

void C_SetColorBG::Run(float tick)
{
	float val = .0f;
	PTR_D3D::ptr->SetColorBG(val, val, val, val);
}

void C_SetColorBG::release()
{
}

C_SetColorBG::C_SetColorBG(string tag):IComp(tag)
{
}

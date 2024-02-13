#include "C_UI.h"

{
	C_UI::vxcs[0].pos = vxs[0];
	vxcs[0].uv = { 0,0 };

	vxcs[1].pos = vxs[1];
	vxcs[1].uv = { 1,0 };

	vxcs[2].pos = vxs[2];
	vxcs[2].uv = { 1,1 };

	vxcs[3].pos = vxs[3];
	vxcs[3].uv = { 0,1 };
}

SimpleInfo_UI::SimpleInfo_UI():tick(0),val0(0),val1(0),val2(0)
{}

void C_UI::initialize()
{
}

void C_UI::release()
{
}

void C_UI::Run(float tick)
{
	C_UI::data[index] = this->info;
	
	DrawCall();
}

C_UI::C_UI(string tag):C_Quad(tag)
{
	info = {};
}

C_UI::C_UI(string tag, Rect2D rect): C_Quad(tag)
{
	info = rect;
}

bool C_UI::IsInside(float x, float y)
{
	return false;
}

Rect2D C_UI::GetRect()
{
	return Rect2D();
}

void C_UI::SetRect(Rect2D rec)
{
}

void C_UI::Flush()
{
}

uint16_t C_UI::GetDrawCount()
{
	return 0;
}

void C_UI::DrawCall()
{
}

uint16_t C_UI::index = 0;
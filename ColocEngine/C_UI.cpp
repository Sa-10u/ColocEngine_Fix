#include "C_UI.h"

	SIMPLEVERTEX C_UI::vxcs[4]=
	{
		{C_UI::vxcs[0].pos = C_Quad::vxs[0],C_UI::vxcs[1].uv = { 0,0 }},
		{C_UI::vxcs[1].pos = C_Quad::vxs[1],C_UI::vxcs[1].uv = { 1,0 }},
		{C_UI::vxcs[2].pos = C_Quad::vxs[2],C_UI::vxcs[1].uv = { 1,1 }},
		{C_UI::vxcs[3].pos = C_Quad::vxs[3],C_UI::vxcs[1].uv = { 0,1 }},
	};
	uint16_t C_UI::index = 0;

	std::array<SimpleQuad, ResourceManager::CBCOUNT> C_UI::data = {};
	std::array<MapBOOL, ResourceManager::CBCOUNT> C_UI::mb = {};

void C_UI::initialize()
{
}

void C_UI::release()
{
}

void C_UI::Run(float tick)
{
	DrawCall();
}

C_UI::C_UI(string tag):C_Quad(tag)
{
	info = {};
}

C_UI::C_UI(string tag, SimpleQuad rect):C_Quad(tag)
{
}

bool C_UI::IsInside(float x, float y)
{
	return  (info.rect.lx < x && x < info.rect.rx) && (info.rect.by < y && y < info.rect.ty) ? true : false;
}

SimpleQuad C_UI::GetInfo()
{
	return SimpleQuad();
}

void C_UI::SetInfo(SimpleQuad rec)
{
	this->info = rec;
}

void C_UI::SetTexture(MapBOOL mb)
{
	this->pic = mb;
}

void C_UI::SetPos(Rect2D rect)
{
	this->info.rect = rect;
}

void C_UI::SetUV(Rect2D rect)
{
	this->info.cutUV = rect;
}

void C_UI::SetTick(float tick)
{
	this->info.tick = tick;
}

void C_UI::SetValue(float v0, float v1, float v2)
{
	this->info.val0 = v0;
	this->info.val1 = v1;
	this->info.val2 = v2;
}

void C_UI::Flush()
{
	index = 0;
}

uint16_t C_UI::GetDrawCount()
{
	return index;
}

void C_UI::DrawCall()
{
	if (index >= ResourceManager::CBCOUNT)	return;

	data[index] = this->info;
	mb[index] = this->pic;

	index++;
}

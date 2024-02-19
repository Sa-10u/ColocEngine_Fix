#include "C_UI.h"

	SIMPLEVERTEX C_UI::vxcs[4]=
	{
		{C_UI::vxcs[0].pos = C_Quad::vxs[0],C_UI::vxcs[1].uv = { 0,0 }},
		{C_UI::vxcs[1].pos = C_Quad::vxs[1],C_UI::vxcs[1].uv = { 1,0 }},
		{C_UI::vxcs[2].pos = C_Quad::vxs[2],C_UI::vxcs[1].uv = { 1,1 }},
		{C_UI::vxcs[3].pos = C_Quad::vxs[3],C_UI::vxcs[1].uv = { 0,1 }},
	};
	uint16_t C_UI::index = 0;

	std::array<SimpleInfo_UI, ResourceManager::CBCOUNT> C_UI::data = {};
	std::array<MapBOOL, ResourceManager::CBCOUNT> C_UI::mb = {};

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
	DrawCall();
}

C_UI::C_UI(string tag):C_Quad(tag)
{
	info = {};
}

C_UI::C_UI(string tag, SimpleInfo_UI rect):C_Quad(tag)
{
}

bool C_UI::IsInside(float x, float y)
{
	return  (info.rect.lx < x && x < info.rect.rx) && (info.rect.by < y && y < info.rect.ty) ? true : false;
}

SimpleInfo_UI C_UI::GetInfo()
{
	return SimpleInfo_UI();
}

void C_UI::SetInfo(SimpleInfo_UI rec)
{
	this->info = rec;
}

void C_UI::SetTexture(MapBOOL mb)
{
	this->pic = mb;
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

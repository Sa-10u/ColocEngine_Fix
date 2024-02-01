#include "Light.h"

enum FLAG
{
	ON = 1,
	DISPOSAL = 2,
	USER_0 = 4,
	USER_1 = 8,
	USER_2 = 16,
	USER_3 = 32,
	USER_4 = 64, 
	USER_5 = 128,
};


P_Light::P_Light()
{
}

bool P_Light::isON(int index)
{
	return comp[index].flag & FLAG::ON;
}

bool P_Light::isDisposal(int index)
{
	return comp[index].flag & FLAG::DISPOSAL;
}

void P_Light::ToOFF(int index)
{
	comp[index].flag &= ~FLAG::ON;
}

void P_Light::ToON(int index)
{
	comp[index].flag |= FLAG::ON;
}

void P_Light::ToDisposal(int index)
{
	comp[index].flag |= FLAG::DISPOSAL;
}

void P_Light::ToStore(int index)
{
	comp[index].flag &= ~FLAG::DISPOSAL;
}

void P_Light::Reset(int index)
{
	comp[index].col = { 0,0,0 };
	comp[index].inten = .0f;
	comp[index].pos = { 0,0,0 };
}

void P_Light::SetFlag(int index, uint8_t flag)
{
	comp[index].flag = flag;
}

void P_Light::SetColor(int index, Color col)
{
	comp[index].col = col;
}

void P_Light::SetIntensity(int index, float inten)
{
	comp[index].inten = inten;
}

void P_Light::SetPosition(int index, Position pos)
{
	comp[index].pos = pos;
}

void* P_Light::pLight()
{
	return this->comp;
}

size_t P_Light::GetSize()
{
	return (sizeof(this->comp[0])) * Lights_MAX;
}

P_Light::lgt P_Light::GetComponent(int index)
{
	return this->comp[index];
}

//------------------------------------------------

D_Light::D_Light()
{
}


bool D_Light::isON(int index)
{
	return comp[index].flag & FLAG::ON;
}

bool D_Light::isDisposal(int index)
{
	return comp[index].flag & FLAG::DISPOSAL;
}

void D_Light::ToOFF(int index)
{
	comp[index].flag &= ~FLAG::ON;
}

void D_Light::ToON(int index)
{
	comp[index].flag |= FLAG::ON;
}

void D_Light::ToDisposal(int index)
{
	comp[index].flag |= FLAG::DISPOSAL;
}

void D_Light::ToStore(int index)
{
	comp[index].flag &= ~FLAG::DISPOSAL;
}

void D_Light::SetFlag(int index, uint8_t flag)
{
	comp[index].flag = flag;
}

void D_Light::Reset(int index)
{
	comp[index].col = { 0,0,0 };
	comp[index].inten = .0f;
	comp[index].pos = { 0,0,0 };
	comp[index].rot = {0,0,0};
	comp[index].thr = .0f;
}

void D_Light::SetColor(int index, Color col)
{
	comp[index].col = col;
}

void D_Light::SetIntensity(int index, float inten)
{
	comp[index].inten = inten;
}

void D_Light::SetPosition(int index, Position pos)
{
	comp[index].pos = pos;
}

void D_Light::SetRotation(int index, Rotation rot)
{
	comp[index].rot = rot;
}

void D_Light::SetThreshold(int index, float thr)
{
	comp[index].thr = thr;
}

void* D_Light::pLight()
{
	return comp;
}

size_t D_Light::GetSize()
{
	return (sizeof(comp[0])) * Lights_MAX;
}

D_Light::lgt D_Light::GetComponent(int index)
{
	return comp[index];
}

A_Light::A_Light()
{
}

bool A_Light::isON(int index)
{
	return comp[index].flag & FLAG::ON;
}

bool A_Light::isDisposal(int index)
{
	return comp[index].flag & FLAG::DISPOSAL;
}

void A_Light::ToOFF(int index)
{
	comp[index].flag &= ~FLAG::ON;
}

void A_Light::ToON(int index)
{
	comp[index].flag |= FLAG::ON;
}

void A_Light::ToDisposal(int index)
{
	comp[index].flag |= FLAG::DISPOSAL;
}

void A_Light::ToStore(int index)
{
	comp[index].flag &= ~FLAG::DISPOSAL;
}

void A_Light::Reset(int index)
{
	comp[index].col = { 0,0,0 };
	comp[index].inten = .0f;
	comp[index].val0 = .0f;
	comp[index].val1 = .0f;
	comp[index].val2 = .0f;
}

void A_Light::SetFlag(int index, uint8_t flag)
{
	comp[index].flag = flag;
}

void A_Light::SetColor(int index, Color col)
{
	comp[index].col = col;
}

void A_Light::SetIntensity(int index, float inten)
{
	comp[index].inten = inten;
}

void* A_Light::pLight()
{
	return comp;
}

size_t A_Light::GetSize()
{
	return (sizeof(comp[0])) * Lights_MAX;
}

A_Light::lgt A_Light::GetComponent(int index)
{
	return comp[index];
}

P_Light::lgt::lgt():col(0,0,0),inten(.0f),pos(0,0,0),flag(FLAG::DISPOSAL)
{
}

D_Light::lgt::lgt() :col(0, 0, 0), inten(.0f), pos(0, 0, 0), flag(FLAG::DISPOSAL),rot(0,0,0),thr(.0f)
{

}

A_Light::lgt::lgt() :col(0, 0, 0), inten(.0f), flag(FLAG::DISPOSAL), val0(.0f), val1(.0f), val2(.0f)
{

}

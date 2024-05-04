#include "S_Light.h"

enum FLAG
{
	USER_0 = 1,
	USER_1 = 2,
	USER_2 = 4,
	USER_3 = 8,
	USER_4 = 16,
	USER_5 = 32,
	USER_6 = 64, 
	USER_7 = 128,
};

Light::Light():IComp(""),index_(NULL)
{}

Light::Light(string tag):IComp(tag),index_(NULL)
{}

//---------------------------------------

P_Light::lgt::lgt():col(NULL,NULL,NULL),flag(NULL),inten(NULL),pos(NULL,NULL,NULL)
{
}

P_Light::P_Light()
{
}

P_Light::P_Light(string tag):Light(tag)
{
	
}

P_Light::~P_Light()
{
}

void P_Light::Reset()
{
	datnum = {};
}

void P_Light::SetFlag(uint8_t flag)
{
	datnum.flag = flag;
}

void P_Light::SetColor(Color col)
{
	datnum.col = col;
}

void P_Light::SetIntensity(float inten)
{
	datnum.inten = inten;
}

void P_Light::SetPosition(Position pos)
{
	datnum.pos = pos;
}

void* P_Light::pLight()
{
	return P_Light::comp;
}

size_t P_Light::GetSize()
{
	return (static_cast<size_t>(sizeof(datnum)) * Lights_MAX);
}

uint16_t P_Light::GetCount()
{
	return P_Light::cnt_;
}

void P_Light::Flush()
{
	cnt_ = 0;
}

void P_Light::initialize()
{
	datnum = {};
	index_ = 0;
}

void P_Light::release()
{
}

void P_Light::Run(float tick)
{
	this->index_ = cnt_;
	cnt_++;

	comp[index_] = this->datnum;
}

D_Light::D_Light()
{
	datnum = {};
}

D_Light::lgt::lgt() :col(NULL, NULL, NULL), flag(NULL), inten(NULL), pos(NULL, NULL, NULL),rot(NULL,NULL,NULL),thr(NULL)
{
}

void D_Light::Reset()
{
	datnum = {};
}

void D_Light::SetFlag(uint8_t flag)
{
	datnum.flag = flag;
}

void D_Light::SetColor(Color col)
{
	datnum.col = col;
}

void D_Light::SetIntensity(float inten)
{
	datnum.inten = inten;
}

void D_Light::SetPosition(Position pos)
{
	datnum.pos = pos;
}

void D_Light::SetRotation(Rotation rot)
{
	datnum.rot = rot;
}

void D_Light::SetThreshold(float thr)
{
	datnum.thr = thr;
}

void* D_Light::pLight()
{
	return D_Light::comp;
}

size_t D_Light::GetSize()
{
	return (sizeof(datnum) * Lights_MAX);
}

uint16_t D_Light::GetCount()
{
	return cnt_;
}

void D_Light::Flush()
{
	cnt_ = 0;
}

void D_Light::initialize()
{
}

void D_Light::release()
{
}

void D_Light::Run(float tick)
{
	index_ = cnt_;
	cnt_++;

	comp[index_] = datnum;
}

A_Light::A_Light()
{
}

A_Light::A_Light(string tag) :Light(tag)
{
}

A_Light::lgt::lgt() :col(NULL, NULL, NULL), flag(NULL), inten(NULL),val0(0),val1(0),val2(0)
{
}

void A_Light::Reset()
{
	datnum = {};
}

void A_Light::SetFlag(uint8_t flag)
{
	datnum.flag = flag;
}

void A_Light::SetColor(Color col)
{
	datnum.col = col;
}

void A_Light::SetIntensity(float inten)
{
	datnum.inten = inten;
}

void* A_Light::pLight()
{
	return A_Light::comp;
}

size_t A_Light::GetSize()
{
	return (sizeof(datnum) * Lights_MAX);
}

uint16_t A_Light::GetCount()
{
	return cnt_;
}

void A_Light::Flush()
{
	cnt_ = 0;
}

void A_Light::initialize()
{
}

void A_Light::release()
{
}

void A_Light::Run(float tick)
{
	index_ = cnt_;
	cnt_++;

	comp[index_] = datnum;
}

//--------------------------

uint16_t P_Light::cnt_ = 0;
P_Light::lgt P_Light::comp[Lights_MAX] = {};

uint16_t D_Light::cnt_ = 0;
D_Light::lgt D_Light::comp[Lights_MAX] = {};

uint16_t A_Light::cnt_ = 0;
A_Light::lgt A_Light::comp[Lights_MAX] = {};
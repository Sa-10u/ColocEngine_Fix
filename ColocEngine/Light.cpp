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

Light::Light():col(Color(1,1,1)),flag(ON)
{
}

Light::~Light()
{
}

bool Light::isON()
{
	return flag & ON;
}

bool Light::isDisposal()
{
	return flag & DISPOSAL;
}

void Light::ToDisposal()
{
	flag &= ~DISPOSAL;
}

void Light::ToOFF()
{
	flag &= ~ON;
}

P_Light::P_Light():pos(Position(0,0,0))
{
}
P_Light::~P_Light()
{
}

D_Light::D_Light():pos(Position(0,0,0)),rot(Rotation(0,0,0)),threshold(0)
{
}
D_Light::~D_Light()
{
}

A_Light::A_Light()
{
}
A_Light::~A_Light()
{
}

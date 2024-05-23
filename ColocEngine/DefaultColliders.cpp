#include "DefaultColliders.h"

bool SphereCol::isHit(Collider* tgt)
{
	if (radius_ <= .0)	return false;
	
	switch (typeid(*tgt).hash_code())
	{
	//case Spherecol_ID :
	
	
	default:	return false;
		break;
	}
}

SphereCol::SphereCol():pos_{0,0,0},radius_{0},parent_(nullptr)
{
}

SphereCol::SphereCol(float3 p, float r) :pos_{p}, radius_{ r }, parent_(nullptr)
{
}

SphereCol::SphereCol(float3 pos, float r, Mat* p):pos_{pos}, radius_{ r }, parent_(p)
{
}

bool SandS()
{
	return false;
}

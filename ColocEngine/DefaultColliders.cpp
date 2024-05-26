#include "DefaultColliders.h"
#include<cmath>

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

bool SandS(float3 c_pos, float c_rad, float3 t_pos, float t_rad)
{
	auto getlenP = [](float3 s, float3 e) {return std::sqrt(pow(e.x - s.x, 2)+ pow(e.y - s.y, 2)+ pow(e.z - s.z, 2)); };

	return getlenP(c_pos, t_pos) < c_rad + t_rad;
}

template<>
uint8_t ColType<SphereCol>()
{
	return static_cast<uint8_t>(ColliderType::Sphere);
}

template<>
uint8_t ColType<BoxCol>()
{
	return static_cast<uint8_t>(ColliderType::Box);

}
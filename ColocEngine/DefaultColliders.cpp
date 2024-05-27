#include "DefaultColliders.h"
#include<cmath>

bool SphereCol::isHit(Collider* tgt)
{
	if (radius_ <= .0f)	return false;
	
	switch (typeid(*tgt).hash_code())
	{
	case static_cast<uint8_t>(ColliderType::Sphere)	:	return SandS(this->pos_,this->radius_,dynamic_cast<SphereCol*>(tgt)->pos_, dynamic_cast<SphereCol*>(tgt)->radius_);
	
	case static_cast<uint8_t>(ColliderType::Box)	:	return BandS(dynamic_cast<BoxCol*>(tgt)->pos_, dynamic_cast<BoxCol*>(tgt)->len_,this->pos_,this->radius_);

	default:	return false;
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

bool BoxCol::isHit(Collider* tgt)
{
	switch (typeid(*tgt).hash_code())
	{
	case static_cast<uint8_t>(ColliderType::Sphere):	return BandS(this->pos_, this->len_, dynamic_cast<SphereCol*>(tgt)->pos_, dynamic_cast<SphereCol*>(tgt)->radius_);

	case static_cast<uint8_t>(ColliderType::Box)	:	return BandB(this->pos_, this->len_, dynamic_cast<BoxCol*>(tgt)->pos_, dynamic_cast<BoxCol*>(tgt)->len_);

	default:	return false;
	}
}

BoxCol::BoxCol()
{
}

BoxCol::BoxCol(float3 pos, float3 len) :pos_{ pos }, len_{ len };
{
}

BoxCol::BoxCol(float3 pos, float3 len, Mat* p) :pos_{ pos }, len_{ len }, parent_{ p }
{
}

//-----------------------------------
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

bool SandS(float3 c_pos, float c_rad, float3 t_pos, float t_rad)
{
	auto getlen = [](float3 s, float3 e) {return std::sqrt(pow(e.x - s.x, 2) + pow(e.y - s.y, 2) + pow(e.z - s.z, 2)); };

	return getlen(c_pos, t_pos) < c_rad + t_rad;
}

bool BandS(float3 bpos, float3 blen, float3 spos, float srad)
{
	return false;
}

bool BandB(float3 c_pos, float3 c_len, float3 t_pos, float3 t_len)
{
	return false;
}
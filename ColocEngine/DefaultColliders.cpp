#include "DefaultColliders.h"
#include<cmath>
#include<DirectXMath.h>

using namespace DirectX;

bool SphereCol::isHit(Collider* tgt)
{
	if (radius_ <= .0f)	return false;
	
	switch (typeid(*tgt).hash_code())
	{
	case static_cast<uint8_t>(ColliderType::Sphere)	:	return SandS(this->pos_,this->radius_,dynamic_cast<SphereCol*>(tgt)->pos_, dynamic_cast<SphereCol*>(tgt)->radius_);
	
	case static_cast<uint8_t>(ColliderType::Box)	:	return BandS(dynamic_cast<BoxCol*>(tgt)->GetPosition(), dynamic_cast<BoxCol*>(tgt)->GetLength(), this->pos_, this->radius_);

	default:	return false;
	}
}

SphereCol::SphereCol() :pos_{0,0,0}, radius_{1}, mat_{ 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 }
{
}

SphereCol::SphereCol(float3 p, float r) :pos_{ p }, radius_{r}, mat_{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }
{
}

void SphereCol::SetPosition(float3 p)
{
	this->pos_ = p;
}

void SphereCol::SetMatrix(Mat m)
{
	mat_ = m;
}

void SphereCol::SetRadius(float r)
{
	radius_ = r;
}

float3 SphereCol::GetPosition()
{
	return pos_;
}

float SphereCol::GetRadius()
{
	return radius_;
}

float3 SphereCol::GetCalcPosition()
{
	float3 res = {};
	XMStoreFloat3(&res, XMVector3Transform(XMLoadFloat3(&pos_), mat_));
}

float SphereCol::GetCalcRadius()
{
	std::max<float>(std::max<float>(XMVectorGetX(mat_.r[0]) , XMVectorGetY(mat_.r[1]) , XMVectorGetZ(mat_.r[3])));
}



bool BoxCol::isHit(Collider* tgt)
{
	switch (typeid(*tgt).hash_code())
	{
	case static_cast<uint8_t>(ColliderType::Sphere):	return BandS(this->pos_, this->len_, dynamic_cast<SphereCol*>(tgt)->GetPosition(), dynamic_cast<SphereCol*>(tgt)->GetRadius());

	case static_cast<uint8_t>(ColliderType::Box)	:	return BandB(this->pos_, this->len_, dynamic_cast<BoxCol*>(tgt)->pos_, dynamic_cast<BoxCol*>(tgt)->len_);

	default:	return false;
	}
}

BoxCol::BoxCol() :len_{ 1,1,1 }, pos_{0,0,0},mat_{1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1}
{
	
}

BoxCol::BoxCol(float3 pos, float3 len) :pos_{ pos }, len_{ len }, mat_{ 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 }
{
}

void BoxCol::SetPosition(float3 p)
{
	pos_ = p;
}

void BoxCol::SetMatrix(Mat m)
{
	mat_ = m;
}

void BoxCol::SetLength(float3 l)
{
	len_ = l;
}

void BoxCol::SetLengthX(float l)
{
	len_.x = l;
}

void BoxCol::SetLengthY(float l)
{
	len_.y = l;
}

void BoxCol::SetLengthZ(float l)
{
	len_.z = l;
}

float3 BoxCol::GetPosition()
{
	return pos_;
}

float3 BoxCol::GetLength()
{
	return len_;
}

float3 BoxCol::GetCalcPosition()
{
	return float3();
}

float3 BoxCol::GetCalcLength()
{
	return float3();
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
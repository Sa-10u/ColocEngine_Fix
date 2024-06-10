#include "DefaultColliders.h"
#include<algorithm>
#include<DirectXMath.h>
#include<cmath>

using namespace DirectX;

bool SphereCol::isHit(Collider* tgt)
{
	if (radius_ <= .0f)	return false;

	auto wldPos = this->GetPositionWLD();
	auto wldRad = this->GetRadiusWLD();
	
	switch (typeid(*tgt).hash_code())
	{
	case static_cast<uint8_t>(ColliderType::Sphere)	:	return SandS(wldPos, wldRad, dynamic_cast<SphereCol*>(tgt)->GetPositionWLD(), dynamic_cast<SphereCol*>(tgt)->GetRadiusWLD());
	
	case static_cast<uint8_t>(ColliderType::Box)	:	return BandS(dynamic_cast<BoxCol*>(tgt)->GetPositionWLD(), dynamic_cast<BoxCol*>(tgt)->GetLengthXWLD(), dynamic_cast<BoxCol*>(tgt)->GetLengthYWLD(), dynamic_cast<BoxCol*>(tgt)->GetLengthZWLD(),wldPos, wldRad);

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

float3 SphereCol::GetPositionLCL()
{
	return pos_;
}

float SphereCol::GetRadiusLCL()
{
	return radius_;
}

float3 SphereCol::GetPositionWLD()
{
	float3 res = {};
	XMStoreFloat3(&res, XMVector3Transform(XMLoadFloat3(&pos_), mat_));
}

float SphereCol::GetRadiusWLD()
{
	return radius_ * (std::max)((std::max)(XMVectorGetX(mat_.r[0]) , XMVectorGetY(mat_.r[1])) , XMVectorGetZ(mat_.r[2]));
}



bool BoxCol::isHit(Collider* tgt)
{
	auto wldPos = this->GetPositionWLD();
	auto wldVecX = this->GetLengthXWLD();
	auto wldVecY = this->GetLengthYWLD();
	auto wldVecZ = this->GetLengthZWLD();

	switch (typeid(*tgt).hash_code())
	{
	case static_cast<uint8_t>(ColliderType::Sphere):	return BandS(wldPos, wldVecX,wldVecY,wldVecZ ,dynamic_cast<SphereCol*>(tgt)->GetPositionWLD(), dynamic_cast<SphereCol*>(tgt)->GetRadiusWLD());

	case static_cast<uint8_t>(ColliderType::Box)	:	return BandB(wldPos, wldVecX,wldVecY,wldVecZ, dynamic_cast<BoxCol*>(tgt)->GetPositionWLD(), dynamic_cast<BoxCol*>(tgt)->GetLengthXWLD(), dynamic_cast<BoxCol*>(tgt)->GetLengthYWLD(), dynamic_cast<BoxCol*>(tgt)->GetLengthZWLD());

	default:	return false;
	}
}

BoxCol::BoxCol() : pos_{ 0,0,0 }, mat_{ 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 }, vecX_{ .5,0,0 }, vecY_{ 0,.5,0 }, vecZ_{ 0,0,.5 }
{
}

BoxCol::BoxCol(float3 pos, float3 len) :pos_{ pos }, mat_{ 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 }, vecX_{ len.x,0,0 }, vecY_{ 0,len.y,0 }, vecZ_{ 0,0,len.z }
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
	vecX_.x = l.x;
	vecY_.y = l.y;
	vecZ_.z = l.z;
}

void BoxCol::SetLengthX(float l)
{
	vecX_.x = l;
}

void BoxCol::SetLengthY(float l)
{
	vecY_.y = l;
}

void BoxCol::SetLengthZ(float l)
{
	vecZ_.z = l;
}

float3 BoxCol::GetPositionLCL()
{
	return pos_;
}

float3 BoxCol::GetLengthXLCL()
{
	return vecX_;
}

float3 BoxCol::GetLengthYLCL()
{
	return vecY_;
}

float3 BoxCol::GetLengthZLCL()
{
	return vecZ_;
}


float3 BoxCol::GetPositionWLD()
{
	float3 res = {};
	XMStoreFloat3(&res, XMVector3Transform(XMLoadFloat3(&pos_), mat_));
}

float3 BoxCol::GetLengthXWLD()
{
	float3 res = {};
	XMStoreFloat3(&res, XMVector3Transform(XMLoadFloat3(&vecX_), mat_));
}

float3 BoxCol::GetLengthYWLD()
{
	float3 res = {};
	XMStoreFloat3(&res, XMVector3Transform(XMLoadFloat3(&vecY_), mat_));
}

float3 BoxCol::GetLengthZWLD()
{
	float3 res = {};
	XMStoreFloat3(&res, XMVector3Transform(XMLoadFloat3(&vecZ_), mat_));
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
	return GetLength(c_pos - t_pos) < c_rad + t_rad;
}

bool BandS(float3 bpos, float3 bx, float3 by, float3 bz, float3 spos, float srad)
{
	float3 nearest = {};//scale for nearest

	auto pos = spos - bpos;
	
	float3 vecs[] = { bx,by,bz };

	for(auto i = 0u; i< _countof(vecs);i++){

		auto way = fl3Normalize(vecs[i]);
		auto len = GetLength(vecs[i]);

		auto scale = Dot(pos, way) / len;

		scale = 1 >= scale >= -1 ? scale : scale > 1 ? 1 : -1;
		nearest += vecs[i] * scale;
	}

	return (GetLength(nearest) + srad) >= GetLength(pos) ? false : true;
}

bool BandB(float3 c_pos, float3 cx, float3 cy, float3 cz, float3 t_pos, float3 tx, float3 ty, float3 tz)
{
	float3 bbvec = t_pos - c_pos;
	float bblenS = GetLengthSquared(bbvec);

	{
		auto getMax = [](float3 fl) {return (std::max)((std::max)(fl.x, fl.y), fl.z); };

		float cmax = getMax(float3(GetLengthSquared(cx),GetLengthSquared(cy),GetLengthSquared(cz)));
		float tmax = getMax(float3(GetLengthSquared(tx), GetLengthSquared(ty), GetLengthSquared(tz)));

		if (bblenS > (cmax + tmax))	return false;
	}

	constexpr uint8_t calcTime_vecC = 3;
	constexpr uint8_t calcTime_vecT = 3;
	constexpr uint8_t calcTime_cross = 9;

	{

		float3 base_c[] =
		{
			cx,
			cy,
			cz
		};

		for (auto i = 0; i < calcTime_vecC; i++) {

			if (Dot(bbvec, base_c[i]) > GetLength(base_c[i]) + (Dot(tx, base_c[i]) + Dot(ty, base_c[i]) + Dot(tz, base_c[i])))	return false;
		}
	
		float3 base_t[] =
		{
			tx,
			ty,
			tz
		};

		for (auto i = 0; i < calcTime_vecT; i++) {

			if (Dot(bbvec, base_t[i]) > GetLength(base_t[i]) + (Dot(cx, base_t[i]) + Dot(cy, base_t[i]) + Dot(cz, base_t[i])))	return false;
		}

		for (auto i = 0; i < calcTime_vecT; i++) {
			for (auto j = 0; j < calcTime_vecC; i++) {

				auto axisVec = Cross(base_c[j], base_t[i]);

				auto getLen = [&](float3& x, float3& y, float3& z)
					{
						return Dot(axisVec, x) + Dot(axisVec, y) + Dot(axisVec, z);
					};

				if (Dot(bbvec, axisVec) > getLen(cx,cy,cz) + getLen(tx,ty,tz))	return false;
			}
		}
	}

	return true;
}
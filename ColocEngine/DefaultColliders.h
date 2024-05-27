#pragma once
#include"MACRO.h"
#include<typeinfo>

class Collider
{
public:
	virtual bool isHit(Collider* col) = 0;
};

class SphereCol : public Collider
{
public:
	bool isHit(Collider* tgt) override;

	SphereCol();
	SphereCol(float3 p, float r);
	SphereCol(float3 pos, float r, Mat* p);


	float3 pos_;
	float radius_;
	Mat* parent_;
};

class BoxCol : public Collider
{
public:
	bool isHit(Collider* col) override;
	BoxCol();
	BoxCol(float3 pos,float3 len);
	BoxCol(float3 pos, float3 len , Mat* p);


	float3 pos_;
	float3 len_;

	Mat* parent_;
};

//-----------------

enum class ColliderType
{
	Sphere =0,
	Box,
	Capsule,
	Triangle,
	Plane,

};

static bool SandS(float3 c_pos,float c_rad , float3 t_pos , float t_rad);
static bool BandS(float3 bpos, float3 blen,float3 spos , float srad);
static bool BandB(float3 c_pos,float3 c_len, float3 t_pos, float3 t_len);

//-----------------
template<typename t>
uint8_t ColType()
{
	return -1;
}

template<>
uint8_t ColType<SphereCol>();
template<>
uint8_t ColType<BoxCol>();
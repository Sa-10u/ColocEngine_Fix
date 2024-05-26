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

private:

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

private:
	float3 pos_;
	float3 radius_;

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
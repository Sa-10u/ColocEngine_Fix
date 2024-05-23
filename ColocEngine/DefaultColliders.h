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

static constexpr size_t Spherecol_ID = typeid(SphereCol).hash_code();
static constexpr size_t Boxcol_ID = typeid(BoxCol).hash_code();

static bool SandS(float3 c_pos,float c_rad , float3 t_pos , float t_rad);
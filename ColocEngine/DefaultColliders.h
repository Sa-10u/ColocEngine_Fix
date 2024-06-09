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
	void SetPosition(float3 p);
	void SetMatrix(Mat m);
	void SetRadius(float r);

	float3 GetPositionLCL();
	float GetRadiusLCL();
	float3 GetPositionWLD();
	float GetRadiusWLD();

private:
	float3 pos_;
	float radius_;
	Mat mat_;
};

class BoxCol : public Collider
{
public:
	bool isHit(Collider* col) override;
	BoxCol();
	BoxCol(float3 pos,float3 len);

	void SetPosition(float3 p);
	void SetMatrix(Mat m);
	void SetLength(float3 l);
	void SetLengthX(float l);
	void SetLengthY(float l);
	void SetLengthZ(float l);

	float3 GetPositionLCL();
	float3 GetLengthXLCL();
	float3 GetLengthYLCL();
	float3 GetLengthZLCL();
	float3 GetPositionWLD();
	float3 GetLengthXWLD();
	float3 GetLengthYWLD();
	float3 GetLengthZWLD();

private:
	float3 pos_;
	float3 vecX_;
	float3 vecY_;
	float3 vecZ_;

	Mat mat_;
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
static bool BandS(float3 bpos, float3 bx,float3 by ,float3 bz,float3 spos , float srad);
static bool BandB(float3 c_pos,float3 cx, float3 cy, float3 cz, float3 t_pos, float3 tx, float3 ty, float3 tz);

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
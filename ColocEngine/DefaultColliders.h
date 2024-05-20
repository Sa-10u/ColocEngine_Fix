#pragma once
#include"MACRO.h"

class Collider
{
public:
	virtual bool isHit(Collider* col) = 0;
};

class SphereCol : public Collider
{
public:
	bool isHit(Collider* col);

	SphereCol();
	SphereCol(float3 p, float r);

private:

	float3 pos;
	float radius;
};

class BoxCol : public Collider
{
public:
	bool isHit(Collider* col);

private:
	float3 pos;
	float x_rad;
	float y_rad;
	float z_rad;
};
#pragma once
#include <cstdint>
#include"C_Trans.h"

typedef DirectX::XMFLOAT3 Color;

struct Light
{
	Light();
	virtual ~Light() = 0;

	bool isON();
	bool isDisposal();

	void ToOFF();
	void ToDisposal();

	Color col;
	int flag;
	float intensity;
};

struct P_Light : Light
{
	P_Light();
	~P_Light();

	Position pos;
};

struct D_Light : Light
{
	D_Light();
	~D_Light();

	Position pos;
	Rotation rot;
	float threshold;
};

struct A_Light : Light
{
	A_Light();
	~A_Light();
};
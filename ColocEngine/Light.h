#pragma once
#include <cstdint>
#include"C_Trans.h"

typedef DirectX::XMFLOAT4 Color;

struct Light
{
	Light();
	virtual ~Light();

	bool isON();
	bool isDisposal();

	Color col;

	short flag;
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
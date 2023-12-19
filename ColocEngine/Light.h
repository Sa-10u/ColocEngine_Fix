#pragma once
#include <cstdint>
#include"C_Trans.h"

typedef DirectX::XMFLOAT4 Color;

struct Light
{
	Light();
	virtual ~Light();

	bool isON;
	Color col;
};

struct P_Light : Light
{
	P_Light();
	~P_Light();

	Position pos;
};
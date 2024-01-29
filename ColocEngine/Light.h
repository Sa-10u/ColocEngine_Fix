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
	float intensity;
	int flag;
};

struct P_Light : Light
{
	P_Light();
	~P_Light() override;

	Position pos;

	struct lgt
	{
		Color col;
		float inten;

		int flag;
		Position pos;
	}comp;
};

struct D_Light : Light
{
	D_Light();
	~D_Light() override;

	Position pos;
	Rotation rot;
	float threshold;

	struct lgt
	{
		Color col;
		float inten;

		int flag;
		Position pos;

		Rotation rot;
		float thr;
	}comp;
};

struct A_Light : Light
{
	A_Light();
	~A_Light() override;

	float val0;
	float val1;
	float val2;

	struct lgt
	{
		Color col;
		float inten;

		int flag;
		float val0;
		float val1;
		float val2;
	}comp;
};
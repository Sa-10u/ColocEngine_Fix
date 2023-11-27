#pragma once
#include "IComp.h"
#include <DirectXMath.h>

typedef DirectX::XMFLOAT3 Position;
typedef DirectX::XMFLOAT3 Rotation;
typedef DirectX::XMFLOAT3 Scale;



struct C_Trans : public IComp
{
	Position pos;
	Rotation rot;
	Scale	 scale;
	C_Trans* parent;

	C_Trans(string tag);

	void initialize() override;
	void release() override;
	void Run(float tick) override;

	DirectX::XMMATRIX WLDGetMTX();
	DirectX::XMMATRIX LCLGetMTX();
};


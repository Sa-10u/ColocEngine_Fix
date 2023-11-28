#pragma once
#include"C_Trans.h"
#include<DirectXMath.h>

namespace CAM
{
	extern DirectX::XMVECTOR Pos;
	extern DirectX::XMVECTOR Tgt;
	extern DirectX::XMVECTOR Head;

	extern float Fov;
	extern float Aspect;

	void Init();
	void Run();
};


#include "CAM.h"
#include"MACRO.h"
#include"D3D.h"
#include<numbers>

using namespace DirectX;

namespace CAM
{
	XMVECTOR Pos;
	XMVECTOR Tgt;
	XMVECTOR Head;

	float Fov;
	float Aspect;
};

void CAM::Init()
{
	Pos = XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
	Tgt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	Head = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	Fov = 37.5 * ( std::numbers::pi/ 180);
	
	Aspect = PTR_D3D::ptr->GetWidth()/PTR_D3D::ptr->GetHeight();
}

void CAM::Run()
{
	Aspect = PTR_D3D::ptr->GetWidth() / PTR_D3D::ptr->GetHeight();
}
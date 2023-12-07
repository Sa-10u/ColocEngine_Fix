#include "C_Trans.h"
using namespace DirectX;

C_Trans::C_Trans(string tag) : IComp(tag)
{
	pos = {};
	rot = {};
	scale = XMFLOAT3(1,1,1);
	parent = nullptr;
}

void C_Trans::initialize()
{
	
}

void C_Trans::release()
{
}

void C_Trans::Run(float tick)
{
}

DirectX::XMMATRIX C_Trans::WLDGetMTX()
{
	if (parent == nullptr)	return this->LCLGetMTX();

	return (this->LCLGetMTX() * parent->WLDGetMTX());
}

DirectX::XMMATRIX C_Trans::LCLGetMTX()
{
	XMMATRIX matP = {
		1,		0,		0,		0,
		0,		1,		0,		0,
		0,		0,		1,		0,
		pos.x,	pos.y,	pos.z,	1,
	};

	XMMATRIX matR;
	{
		float C = rot.x;
		XMMATRIX matRX = {
			1,		0,		0,		 0,
			0,		cosf(C),-sinf(C),0,
			0,		sinf(C),cosf(C), 0,
			0,		0,		0,		 1,
		};

		C = rot.y;
		XMMATRIX matRY = {
			cosf(C),0,	   -sinf(C),0,
			0,		1,		0,		0,
			sinf(C),0,		cosf(C),0,
			0,		0,		0,		1,
		};

		C = rot.z;
		XMMATRIX matRZ = {
			cosf(C),sinf(C),0,		0,
		   -sinf(C),cosf(C),0,		0,
			0,		0,		1,		0,
			0,		0,		0,		1,
		};

		matR = matRX * matRY * matRZ;
	}

	XMMATRIX matS = {
	scale.x,0,		0,		0,
	0,		scale.y,0,		0,
	0,		0,		scale.z,0,
	0,		0,		0,		1,
	};

	return (matR * matS * matP);
}

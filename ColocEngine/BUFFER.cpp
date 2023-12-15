#include "BUFFER.h"
#include<assert.h>

VERTEX::VERTEX(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT2 const& uv, XMFLOAT3 const& tan):pos_(pos),norm_(norm),uv_(uv),tan_(tan)
{
}

MESH::MESH():ID_Material(-1),indexes_(),vtcs_()
{
}
//--------

ObjInfo::ObjInfo():tick(0),val0(0),val1(0),val2(0)
{
	wld = 
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
}

MATERIAL::MATERIAL():alpha_(0),shin_(0)
{
	dif_ = {};
	spec_ = {};
	dmap_ = {};
}

MapBOOL::MapBOOL():isD(NULL),isS(NULL),isE(NULL),isN(NULL),isESB(NULL)
{
}

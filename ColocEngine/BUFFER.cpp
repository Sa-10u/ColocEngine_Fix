#include "BUFFER.h"
#include<assert.h>
#include"MACRO.h"

VERTEX::VERTEX(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT2 const& uv, XMFLOAT3 const& tan ,UINT const& MID)
	:pos_(pos),norm_(norm),uv_(uv),tan_(tan),mtlID_(MID)
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


static auto fmt1 = DXGI_FORMAT_R32G32B32A32_UINT;
static auto fmt3 = DXGI_FORMAT_R32G32B32_FLOAT;
static auto fmt2 = DXGI_FORMAT_R32G32_FLOAT;
static auto append = D3D12_APPEND_ALIGNED_ELEMENT;
static auto IL_V = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

const D3D12_INPUT_ELEMENT_DESC VERTEX::element[] =
{
	{SEMANTICS_STR::POSITION,0,fmt3,0,append,IL_V,0},
	{SEMANTICS_STR::NORMAL,0,fmt3,0,append,IL_V,0},
	{SEMANTICS_STR::TEXCOORD,0,fmt2,0,append,IL_V,0},
	{SEMANTICS_STR::TANGENT,0,fmt3,0,append,IL_V,0},
	{SEMANTICS_STR::MATERIAL,0,fmt1,0,append,IL_V ,0},
};
const D3D12_INPUT_LAYOUT_DESC VERTEX::inp_Layout =
{
	VERTEX::element,
	VERTEX::ELEMENT::AMMOUNT
};

const D3D12_INPUT_ELEMENT_DESC SIMPLEVERTEX::element[] =
{
	{SEMANTICS_STR::POSITION,0,fmt3,0,append,IL_V,0},
	{SEMANTICS_STR::TEXCOORD,0,fmt2,0,append,IL_V,0},
};

const D3D12_INPUT_LAYOUT_DESC SIMPLEVERTEX::inp_Layout =
{
	SIMPLEVERTEX::element,
	SIMPLEVERTEX::AMMOUNT
};
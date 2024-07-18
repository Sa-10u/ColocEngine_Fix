#include "BUFFER.h"
#include<assert.h>
#include"MACRO.h"

VERTEX::VERTEX(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT2 const& uv, XMFLOAT3 const& tan, XMFLOAT3 const& bitan,UINT const& MID)
	:pos_(pos),norm_(norm),uv_(uv),tan_(tan),bitan_(bitan),mtlID_(MID)
{
}

MESH::MESH():ID_Material(-1),indexes_(),vtcs_()
{
}
//--------

MapBOOL::MapBOOL():isD(NULL),isS(NULL),isE(NULL),isN(NULL),isESB(NULL),val0(.0f),val1(.0f),val2(.0f)
{
}

static auto fmt4 = DXGI_FORMAT_R32G32B32A32_UINT;
static auto fmt3 = DXGI_FORMAT_R32G32B32_FLOAT;
static auto fmt2 = DXGI_FORMAT_R32G32_FLOAT;
static auto append = D3D12_APPEND_ALIGNED_ELEMENT;
static auto IP_V = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
static auto fmtfl4 = DXGI_FORMAT_R32G32B32A32_FLOAT;

const D3D12_INPUT_ELEMENT_DESC VERTEX::element[] =
{
	{SEMANTICS_STR::POSITION,0,fmt3,0,append,IP_V,0},
	{SEMANTICS_STR::NORMAL,0,fmt3,0,append,IP_V,0},
	{SEMANTICS_STR::TEXCOORD,0,fmt2,0,append,IP_V,0},
	{SEMANTICS_STR::TANGENT,0,fmt3,0,append,IP_V,0},
	{SEMANTICS_STR::BITANGENT,0,fmt3,0,append,IP_V,0},
	{SEMANTICS_STR::MATERIAL,0,fmt4,0,append,IP_V ,0},
	{SEMANTICS_STR::BONEIDs,0,fmt4,0,append,IP_V,0 },
	{SEMANTICS_STR::BONEWEIGHTs,0,fmtfl4,0,append,IP_V,0}
};
const D3D12_INPUT_LAYOUT_DESC VERTEX::inp_Layout =
{
	VERTEX::element,
	VERTEX::ELEMENT::AMMOUNT
};

const D3D12_INPUT_ELEMENT_DESC SIMPLEVERTEX::element[] =
{
	{SEMANTICS_STR::POSITION,0,fmt3,0,append,IP_V,0},
	{SEMANTICS_STR::TEXCOORD,0,fmt2,0,append,IP_V,0},
};

const D3D12_INPUT_LAYOUT_DESC SIMPLEVERTEX::inp_Layout =
{
	SIMPLEVERTEX::element,
	SIMPLEVERTEX::AMMOUNT
};

Rect2D::Rect2D(float lx, float ty, float rx, float by):lx(lx),ty(ty),rx(rx),by(by)
{
}

Rect2D::Rect2D():lx(0),ty(0),rx(1),by(1)
{
}

SimpleQuad::SimpleQuad() :tick(0), val0(0), val1(0), val2(0)
{}

AnimationData_BONE::AnimationData_BONE():startPos(NULL),endPos(NULL)
{
}

BONE_INFO::BONE_INFO() :parent_(-1), bind_{}, current_{}
{
}

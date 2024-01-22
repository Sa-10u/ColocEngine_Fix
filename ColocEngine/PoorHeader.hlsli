static const uint MAX_LIGHT = 256;

struct VSinput
{
	float3 pos	: POSITION;
    float3 norm : NORMAL;
	float2 uv	: TEXCOORD;
    float3 tan  : TANGENT;
	float3 bitan: BITANGENT;

	uint MID	: MATERIAL_ID;
	
	uint ID		: SV_InstanceID;
};

struct VSoutput
{
	float4 Ppos	: SV_POSITION;
	float3 Wpos	: POSITION;
	float2 uv	: TEXCOORD;
	float3 tan  : TANGENT;
	float3 norm : NORMAL;
	float3 bitan: BITANGENT;

	uint MID	: MATERIAL_ID;
	uint ID		: InstanceID;
};

struct PSoutput
{
	float4 color : SV_TARGET0;
};
//--------------------------

struct PerInstance
{
	float4x4 World;
	float tick;

	float val0;
	float val1;
	float val2;
};

struct MapBOOL
{
	int isD;
	int isS;
	int isE;
	int isN;
	int isESB;

	float val0;
	float val1;
	float val2;
};

struct Material
{
	float3 dif;
	float shin;

	float3 spec;
	float alp;

	float3 emis;
	float val0;	
};

//-------------------

struct P_LIGHT
{
	float3 col;
	float inten;
	float3 pos;
	int flag;
};

struct D_LIGHT
{
	float3 col;
	float inten;
	float3 pos;
	int flag;
};

struct A_LIGHT
{
	float3 col;
	float inten;
	int flag;
};

//------------------------

cbuffer Util : register(b0)
{
	float4x4 View		: packoffset(c0);
	float4x4 Projection : packoffset(c4);
	float Time : packoffset(c8);
};

cbuffer CAM	:register(b256)
{
	float3 pos;
	float3 tgt;
};

cbuffer Light_cnt : register(b512)
{
	uint point_cnt;
	uint dir_cnt;
	uint amb_cnt;
	uint area_cnt;
	uint vol_cnt;
	uint spot_cnt;
}

cbuffer Lights : register (b768)
{
	P_LIGHT pl[MAX_LIGHT];
	D_LIGHT dl[MAX_LIGHT];
	A_LIGHT al[MAX_LIGHT];
};

StructuredBuffer<PerInstance> Data	: register(t0);
StructuredBuffer<MapBOOL>	Map	: register(t512);
StructuredBuffer<Material>  Mtl : register(t1024);

SamplerState colsmp : register(s0);
Texture2D	 colmap[] : register(t1792);




struct VSinput
{
	float3 pos	: POSITION;
    float3 norm : NORMAL;
	float2 uv	: TEXCOORD;
    float3 tan  : TANGENT;

	uint MID	: MATERIAL_ID;
	
	uint	ID	: SV_InstanceID;
};

struct VSoutput
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD;
	float3 tan  : TANGENT;
	float3 norm : NORMAL;

	uint MID	: MATERIAL_ID;
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

StructuredBuffer<PerInstance> Data	: register(t0);
StructuredBuffer<MapBOOL>	Map	: register(t512);
StructuredBuffer<Material>  Mtl : register(t1024);

SamplerState colsmp : register(s0);
Texture2D	 colmap : register(t1792);




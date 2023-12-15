struct VSinput
{
	float3 pos	: POSITION;
    float3 norm : NORMAL;
	float2 uv	: TEXCOORD;
    float3 tan  : TANGENT;
	
	uint	ID	: SV_InstanceID;
};

struct VSoutput
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD;
	float3 tan  : TANGENT;
	float3 norm : NORMAL;
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

//------------------------
cbuffer Material : register(b1)
{
	float3 dif;
	float3 spec;
	float3 emis;
	float alp;
	float shin;
};

cbuffer Util : register(b0)
{
	float4x4 View		: packoffset(c0);
	float4x4 Projection : packoffset(c4);
	float Time : packoffset(c8);
};

cbuffer CAM	:register(b0, space1)
{
	float3 pos;
	float3 tgt;
};

StructuredBuffer<PerInstance> Data	: register(t1 , space0);
StructuredBuffer<MapBOOL>	Map	: register(t1 , space1);

SamplerState colsmp : register(s0);
Texture2D	 colmap : register(t0 ,space0);




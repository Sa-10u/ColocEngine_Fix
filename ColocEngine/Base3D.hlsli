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
    uint VID	: SV_VertexID;
	uint offset : SV_StartInstanceLocation;
};

struct VSoutput
{
	float4 Ppos	: SV_POSITION;
	float4 Wpos	: POSITION;
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
    float4 normal: SV_Target1;
    float4 emission : SV_Target2;
    float4 depth : SV_Target3;
    float4 position : SV_Target4;
    float4 t0 : SV_Target5;
    float4 t1 : SV_Target6;
    float4 t2 : SV_Target7;
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
	float emis_str;	
};

cbuffer CAM	:register(b256)
{
	float3 Cpos;
	float3 Ctgt;
};

cbuffer Util : register(b0)
{
    float4x4 View : packoffset(c0);
    float4x4 Projection : packoffset(c4);
    float Time : packoffset(c8);
    uint PL_CNT : packoffset(c12);
    uint DL_CNT : packoffset(c16);
    uint AL_CNT : packoffset(c20);
};

StructuredBuffer<PerInstance> Data	: register(t0);
StructuredBuffer<MapBOOL>	Map	: register(t512);
StructuredBuffer<Material>  Mtl : register(t1024);

Texture2D colmap[] : register(t1792);

SamplerState colsmp : register(s0);

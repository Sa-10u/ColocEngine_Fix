struct VSinput
{
    float3 pos	: POSITION;
    float2 uv	: TEXCOORD;

    uint ID		: SV_InstanceID;
    uint VID    : SV_VertexID;
};

struct VSoutput
{
    float4 pos  : SV_POSITION;
    float2 uv   : TEXCOORD;

    uint ID	    : InstanceID;
    uint VID    : VertexID;
};

struct PSoutput
{
    float4 color : SV_TARGET0;
    float4 normal : SV_Target1;
    float4 emission : SV_Target2;
    float4 depth : SV_Target3;
    float4 position : SV_Target4;
    float4 t0 : SV_Target5;
    float4 t1 : SV_Target6;
    float4 t2 : SV_Target7;
};

struct Quad
{
    float x;
    float y;
};

//--------------------
struct PerInstance
{ 
    Quad pos_st;
    Quad pos_end;

    Quad uv_st;
    Quad uv_end;

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

cbuffer Util : register(b0)
{
    float Time : packoffset(c8);
};

StructuredBuffer<PerInstance> Data : register(t0);
StructuredBuffer<MapBOOL>	Map	: register(t256);

SamplerState colsmp : register(s0);
Texture2D	 colmap[] : register(t512);
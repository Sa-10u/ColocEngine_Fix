SamplerState colsmp : register(s0);
Texture2D	 colmap[] : register(t256);

struct VSinput
{
    float3 pos	: POSITION;
    float2 uv	: TEXCOORD;
};

struct VSoutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

struct PSoutput
{
    float4 col : SV_TARGET0;
};

//--------------------

cbuffer Util : register(b0)
{
    float Time : packoffset(c8);
};

struct PerInstance
{ 
    float tick;
    float val0;
    float val1;
    float val2;

    float4 clip;
};

StructuredBuffer<PerInstance> Data : register(t0);
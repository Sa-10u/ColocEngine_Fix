Texture2D raw : register(t0);
SamplerState smp : register(s0);

struct VSinput
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
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

//-------------------------------

cbuffer Util : register(b0)
{
    float4x4 View		: packoffset(c0);
    float4x4 Projection : packoffset(c4);
    float Time : packoffset(c8);
};
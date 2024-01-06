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
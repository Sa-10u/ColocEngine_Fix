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

cbuffer Util : register(b0)
{
	float4x4 View		: packoffset(c0);
	float4x4 Projection : packoffset(c4);
    float Time			: packoffset(c8);
};

struct PerInstance
{
	float4x4 World;
	float tick;

	float val0;
	float val1;
	float val2;
};

StructuredBuffer<PerInstance> Data	: register(t1);



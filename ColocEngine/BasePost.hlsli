struct VSinput
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;

    uint ID : SV_InstanceID;
    uint VID : SV_VertexID;
};

struct VSoutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;

    uint ID : InstanceID;
    uint VID : VertexID;
};

struct PSoutput
{
    float4 col : SV_TARGET0;
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

//-------------------------------

static const uint Num_Render = 8;

cbuffer Util : register(b0)
{
    float4x4 View		: packoffset(c0);
    float4x4 Projection : packoffset(c4);
    float Time : packoffset(c8);
};

StructuredBuffer<MapBOOL>	Map	: register(t8);

Texture2D R_Color : register(t0);
Texture2D R_Normal : register(t1);
Texture2D R_Emission : register(t2);
Texture2D R_Depth : register(t3);
Texture2D R_Position : register(t4);
Texture2D R_t0 : register(t5);
Texture2D R_t1 : register(t6);
Texture2D R_t2 : register(t7);

Texture2D raw[] : register(t520);
SamplerState smp : register(s0);




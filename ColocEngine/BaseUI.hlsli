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
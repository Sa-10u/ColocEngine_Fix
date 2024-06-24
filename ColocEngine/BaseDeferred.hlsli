struct VSinput
{
    float3 pos: POSITION;
    float2 uv : TEXCOORD;

    uint ID   : SV_InstanceID;
    uint VID  : SV_VertexID;
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

Texture2D R_Color : register(t0);
Texture2D R_Normal : register(t1);
Texture2D R_Emission : register(t2);
Texture2D R_Depth : register(t3);
Texture2D R_Position : register(t4);
Texture2D R_t0 : register(t5);
Texture2D R_t1 : register(t6);
Texture2D R_t2 : register(t7);

Texture2D raw[] : register(t8);
SamplerState smp : register(s0);

//-------------------

struct P_LIGHT
{
    float3 color;
    float inten;

    float3 pos;
    int flag;
};

struct D_LIGHT
{
    float3 color;
    float inten;

    float3 pos;
    int flag;
	
    float3 rot;
    float thr;
};

struct A_LIGHT
{
    float3 color;
    float inten;
	
    int flag;
    float val0;
    float val1;
    float val2;
};

//------------------------

cbuffer Util : register(b0)
{
    float4x4 View : packoffset(c0);
    float4x4 Projection : packoffset(c4);
    float Time : packoffset(c8);
    uint PL_CNT : packoffset(c12);
    uint DL_CNT : packoffset(c16);
    uint AL_CNT : packoffset(c20);
};


cbuffer CAM : register(b256)
{
    float3 Cpos;
    uint resol_x;
    float3 Ctgt;
    uint resol_y;
};

static const uint MAX_LIGHT = 256;
cbuffer Lights : register(b512) 
{
    P_LIGHT pl[MAX_LIGHT];
    D_LIGHT dl[MAX_LIGHT];
    A_LIGHT al[MAX_LIGHT];
};
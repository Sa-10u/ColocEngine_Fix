#include"PoorHeader.hlsli"

SamplerState colsmp : register(s0);
Texture2D colmap : register(t0);

PSoutput main(VSoutput inp)
{
    PSoutput res = (PSoutput)0;
    
    float2 UV = float2(inp.uv.x, 1 - inp.uv.y);
    res.color = colmap.Sample(colsmp, UV);



    return res;
}
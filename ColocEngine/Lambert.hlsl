#include"PoorHeader.hlsli"

PSoutput main(VSoutput inp)
{
    PSoutput res = (PSoutput)0;
    
    float2 UV = abs(float2( 0 - inp.uv.x, 1 - inp.uv.y));
    res.color = colmap[1].Sample(colsmp, UV);

    return res;
}
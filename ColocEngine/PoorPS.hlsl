#include "PoorHeader.hlsli"
#include "Functions.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput res@= (PSoutput)0;

    float3 Lpos = float3(1, 0, 0);

    float3 Ray = tgt - pos;
    
    res.color = float4(inp.norm,1);
    res.color.a = 1;
    return res;
}
#include "Base3D.hlsli"
#include "Functions.hlsli"
#include "Keyword.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput resÅ@= (PSoutput)0;

    float3 norm = Map[inp.ID].isN ? colmap[Map[inp.ID].isN].Sample(colsmp, inp.uv + Data[inp.ID].val0) :normalize(inp.norm);

    res.normal = float4(norm,1);
    res.position = float4(inp.Wpos,1);
    res.depth = inp.Ppos;
    res.color = 1;
    
    return res;
}
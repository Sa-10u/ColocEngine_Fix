#include "Base3D.hlsli"

PSoutput main(VSoutput inp)
{
	PSoutput res　= (PSoutput)0;

    res.normal = float4(Map[inp.ID].isN ? 
    colmap[Map[inp.ID].isN].Sample(colsmp, inp.uv).xyz :
    normalize(inp.norm).xyz,1);
    
    res.position = float4(inp.Wpos,1);
    
    res.depth = inp.Ppos;
    
    res.color = float4(Map[inp.ID].isD ?
    colmap[Map[inp.ID].isD].Sample(colsmp,inp.uv).xyz:
    Mtl[inp.MID].dif, 1);
    
    res.emission = float4(Map[inp.ID].isE ?
    colmap[Map[inp.ID].isE].Sample(colsmp, inp.uv) :
    float4(Mtl[inp.MID].emis, Mtl[inp.MID].emis_str));
    
    if (inp.ID == 0)
        res.color = 4;
    
    return res;
}
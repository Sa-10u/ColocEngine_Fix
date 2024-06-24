#include "Base3D.hlsli"
#include"Functions.hlsli"

PSoutput main(VSoutput inp)
{
    float4 color = float4(Map[inp.ID].isD != -1 ?
    colmap[Map[inp.ID].isD].Sample(colsmp, inp.uv) :
    float4(Mtl[inp.MID].dif, Mtl[inp.MID].alp));
    
	PSoutput res = (PSoutput)0;
    
    res.color =Dithering(inp.Ppos.xy, color, uint2(resol_x, resol_y));
    
    res.normal = float4(Map[inp.ID].isN != -1 ? 
    colmap[Map[inp.ID].isN].Sample(colsmp, inp.uv).xyz :
    normalize(inp.norm).xyz,1);
    
    res.position = inp.Wpos;
    
    res.depth = inp.Ppos;
    
    res.emission = float4(Map[inp.ID].isE != -1 ?
    colmap[Map[inp.ID].isE].Sample(colsmp, inp.uv) :
    float4(Mtl[inp.MID].emis, Mtl[inp.MID].emis_str));
    
    return res;
}
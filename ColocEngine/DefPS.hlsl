#include "3DBaseHeader.hlsli"
#include "Functions.hlsli"
#include "Keyword.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput resÅ@= (PSoutput)0;
    
    float difp = 0;
    float mirp = 0;

    float3 norm = normalize(inp.norm);
    float3 eyev = normalize(pos - inp.Wpos);

   for (int i = 0u; i < MAX_LIGHT; i++) {

       float len = length(pl[i].pos - inp.Wpos);
       float3 ltoc = normalize(pl[i].pos - pos);

       //difp += dot(normalzie(pl[i].pos - inp.Wpos), normalize(inp.norm));

       

    }

    for (int i = 0u; i < MAX_LIGHT; i++) {

    }

    for (int i = 0u; i < MAX_LIGHT; i++) {

    }

    res.color = colmap[Map[inp.ID].isD].Sample(colsmp, inp.uv + Map[inp.ID].isN);

    return res;
}
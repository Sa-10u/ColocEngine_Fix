#include "Base3D.hlsli"
#include "Functions.hlsli"
#include "Keyword.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
    PSoutput resÅ@ = (PSoutput)0;

    float difp = 0;
    float mirp = 0;

    float3 norm = Map[inp.ID].isN ? colmap[Map[inp.ID].isN].Sample(colsmp, inp.uv + Data[inp.ID].val0) : normalize(inp.norm);

    norm = mul(norm, Data[inp.ID].World);
    float3 eyev = normalize(Cpos - inp.Wpos);

    float3 Pcol = 0;
    float4 base = colmap[Map[inp.ID].isD].Sample(colsmp, inp.uv + Data[inp.ID].val0);
    res.color.a = base.a;

    for (int i = 0u; i < PL_CNT; i++) {

        float len = length(pl[i].pos - inp.Wpos);
        float3 ltov = normalize(pl[i].pos - inp.Wpos);

        difp += ((dot(normalize(pl[i].pos - inp.Wpos), norm)) * (pow(saturate(1 / len), 2))) * pl[i].inten;
        mirp += (saturate(pow(dot(normalize(ltov + eyev), norm), 40))) * (pl[i].inten * clamp(10 - len, 0, 1));

        Pcol += pl[i].color;
    }

    for (int i = 0u; i < DL_CNT; i++) {
        dl[i];
    }

    float ap = .0f;
    float3 Acol = 0;

    for (int i = 0u; i < AL_CNT; i++) {

        ap += al[i].inten;
        Acol += al[i].color;
    }

    float mu = 5;

    difp = floor((difp) * mu) / mu;
    ap = floor(ap * mu) / mu;

    res.color.rgb = (base) * ((mirp + difp) * Pcol);
    res.color.rgb += (res.color.rgb) + (ap * Acol);

    return res;
}
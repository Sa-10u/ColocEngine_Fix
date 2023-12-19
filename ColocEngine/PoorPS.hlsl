#include "PoorHeader.hlsli"
#include "Functions.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput resÅ@= (PSoutput)0;

    float3 Lpos = float3(1, 0, 0);
    float3 Lvec = normalize(Lpos);

    float3 Ray = tgt - pos;
    float intens = 100;

    float p = 4;
    float rvec = dot(inp.norm, Lvec) * inp.norm;
    float rct =  2 *inp.norm* rvec - Lvec;
    rct = saturate(rct);
    rct = pow(rct, Mtl[inp.MID].shin) * intens;
    rct = saturate(rct);

    float near = 1/pow((distance(inp.Wpos , Lpos )),2);
    near = near;
   
    float ct = saturate(dot(Lvec, inp.norm));
    ct = ct* intens* (near);
    ct = saturate(ct * 0.01);
    
    res.color = float4(Mtl[inp.MID].dif * ct + Mtl[inp.MID].spec * rct, Mtl[inp.MID].alp);

    res.color.a = 1;
    res.color.rgb = res.color.rgb * abs(sin(Time));

    return res;
}